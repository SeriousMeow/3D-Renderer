#include "renderer/renderer.hpp"

#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer/resources_manager.hpp"
#include "renderer/thread_pool.hpp"

namespace renderer {

namespace {
/**
 * 2D точка
 */
using Point2 = glm::vec2;

/**
 * 2D вектор
 */
using Vector2 = glm::vec2;

/**
 * Матрица 3x3
 */
using Matrix3 = glm::mat3;

/**
 * Малое значение
 */
constexpr float kEpsilon = glm::epsilon<float>();

/**
 * @brief Пересечение прямой и плоскости
 *
 * Плоскость задается своим вектором нормали и смещением относительно начала координат. Прямая
 * задается как p + v * t, где p - точка на ней, v - вектор направления. Возвращает такое значение
 * t0, что p + v * t0 является точкой пересечения прямой и плоскости. Требуется, чтобы прямая и
 * плоскость были параллельны
 *
 * @param[in] plane Плоскость
 * @param[in] line_start Точка на прямой (p)
 * @param[in] line_direction Вектор направления (v)
 *
 * @return t0
 */
float PlaneIntersection(const Vector4& plane, const Point& line_start,
                        const Vector& line_direction) {
    float denominator = glm::dot(plane, Vector4{line_direction, 0});
    {
        assert((glm::epsilonNotEqual(denominator, 0.0f, kEpsilon)) and
               "PlaneIntersection: прямая и плоскость не должны быть параллельны");
    }
    return -glm::dot(plane, Vector4{line_start, 1}) / denominator;
}

/**
 * @brief Интерполяция вершины
 *
 * Интерполирует вершину, которая находится в точке start + (end - start) * t по заданным значениям
 * start, end, t
 *
 * @param[in] start Начало
 * @param[in] end Конец
 * @param[in] t Коэффициент t
 *
 * @return Интерполированная вершина
 */
Vertex InterpolateVertex(const Vertex& start, const Vertex& end, const float t) {
    Vertex result;
    result.point = start.point + (end.point - start.point) * t;
    result.normal = start.normal + (end.normal - start.normal) * t;
    result.uv_coordinates = start.uv_coordinates + (end.uv_coordinates - start.uv_coordinates) * t;
    return result;
}

/**
 * @brief Обрезка треугольника относительно плоскости
 *
 * Обрезает треугольник относительно плоскости, заданной вектором нормали и смещением
 * относительно начала координат. Остается часть, лежащая по ту же сторону плоскости, что и
 * нормаль. Эта часть разбивается на треугольники и записывается по переданному указателю,
 * возвращается количество записанных треугольников (0, 1 или 2). Требуется, чтобы по
 * переданному указателю было возможно записать 2 значения
 *
 * @param[in] triangle Треугольник для обрезки
 * @param[in] plane Плоскость
 * @param[out] result Указатель, по которому записывается результат
 *
 * @return Количество записанных треугольников
 */
size_t ClipTriangleAganistPlane(const Triangle& triangle, const Vector4& plane, Triangle* result) {
    {
        assert(result and "ClipTriangleAganistPlane: result не должен быть nullptr");
    }
    Vertex inside[3];
    Vertex outside[3];
    int inside_count = 0;
    int outside_count = 0;

    for (int i = 0; i < 3; ++i) {
        if (glm::dot(plane, Point4{triangle.vertices[i].point, 1}) >= 0) {
            inside[inside_count] = triangle.vertices[i];
            ++inside_count;
        } else {
            outside[outside_count] = triangle.vertices[i];
            ++outside_count;
        }
    }
    if (inside_count == 0) {
        // треугольник полностью удален
        return 0;
    }
    if (inside_count == 3) {
        // треугольник полностью по нужную сторону
        result[0] = triangle;
        return 1;
    }
    if (inside_count == 1) {
        // от треугольника остается один треугольник
        result[0].vertices[0] = inside[0];
        {
            Vector direction = outside[0].point - inside[0].point;
            float t = PlaneIntersection(plane, inside[0].point,
                                        direction);  // t для интерполяции свойств новой вершины
            result[0].vertices[1] = InterpolateVertex(inside[0], outside[0], t);
        }
        {
            Vector direction = outside[1].point - inside[0].point;
            float t = PlaneIntersection(plane, inside[0].point,
                                        direction);  // t для интерполяции свойств новой вершины
            result[0].vertices[2] = InterpolateVertex(inside[0], outside[1], t);
        }
        return 1;
    }
    // от треугольника остается четырехугольник, который разбивается на 2 треугольника
    {
        result[0].vertices[0] = inside[0];
        result[0].vertices[1] = inside[1];
        {
            Vector direction = outside[0].point - inside[0].point;
            float t = PlaneIntersection(plane, inside[0].point,
                                        direction);  // t для интерполяции свойств новой вершины
            result[0].vertices[2] = InterpolateVertex(inside[0], outside[0], t);
        }
    }
    {
        result[1].vertices[0] = inside[0];
        result[1].vertices[1] = result[0].vertices[2];
        {
            Vector direction = outside[0].point - inside[1].point;
            float t = PlaneIntersection(plane, inside[1].point,
                                        direction);  // t для интерполяции свойств новой вершины
            result[1].vertices[2] = InterpolateVertex(inside[1], outside[0], t);
        }
    }
    return 2;
}

/**
 * @brief Вычисление барицентрических координат
 *
 * Вычисляет барицентрические координаты точки p относительно точек a, b, c в 2D
 *
 * @param[in] a Координатная точка
 * @param[in] b Координатная точка
 * @param[in] c Координатная точка
 * @param[in] p Точка для вычисления координат
 *
 * @return Барицентрические координаты
 */
Vector Barycentric(const Point2& a, const Point2& b, const Point2& c, const Point2& p) {
    Vector coordinates;
    Vector2 v0 = b - a;
    Vector2 v1 = c - a;
    Vector2 v2 = p - a;
    float d00 = glm::dot(v0, v0);
    float d01 = glm::dot(v0, v1);
    float d11 = glm::dot(v1, v1);
    float d20 = glm::dot(v2, v0);
    float d21 = glm::dot(v2, v1);
    float denominator = d00 * d11 - d01 * d01;
    coordinates.y = (d11 * d20 - d01 * d21) / denominator;
    coordinates.z = (d00 * d21 - d01 * d20) / denominator;
    coordinates.x = 1.0f - coordinates.y - coordinates.z;
    return coordinates;
}

struct LightParameters {
    Point position;
    Vector normal;
    const Matrix& scene_to_camera;
};

/**
 * Применение матрицы трансформации к точке
 *
 * @param[in] point Точка
 * @param[in] transform Матрица трансформации
 * @param[in] divide_w Требуется ли разделить на координату w
 *
 * @return Трансформированная точка
 */
inline Point TransformPoint(const Point& point, const Matrix& transform, bool divide_w = false) {

    Point4 transformed_point = transform * Point4{point, 1};
    if (divide_w) {
        {
            assert((glm::epsilonNotEqual(transformed_point.w, 0.0f, kEpsilon)) and
                   "Scene: после умножения на матрицу перехода в пространство камеры "
                   "координата w стала 0");
        }
        transformed_point /= transformed_point.w;
    }
    return transformed_point;
}

/**
 * Применение матрицы трансформации к вектору
 *
 * @param[in] point Точка
 * @param[in] transform Матрица трансформации
 *
 * @return Трансформированный вектор
 */
inline Vector TransformVector(const Vector& vector, const Matrix& transform) {
    Vector4 transformed_vector = transform * Vector4{vector, 0};
    return transformed_vector;
}

/**
 * Применение матрицы трансформации к вектору
 *
 * @param[in] point Точка
 * @param[in] transform Матрица трансформации
 *
 * @return Трансформированный вектор
 */
inline Vector TransformVector(const Vector& vector, const Matrix3& transform) {
    Vector transformed_vector = transform * vector;
    return transformed_vector;
}

/**
 * Рассчет модификатора яркости света от расстояния
 */
inline float DistantStrength(const PointLight& light_source, const float distance) {
    return 1.0f / (light_source.constant + light_source.linear * distance +
                   light_source.quadratic * distance * distance);
}

/**
 * Рассчет модификатора яркости света от расстояния
 */
inline float DistantStrength(const SpotLight& light_source, const float distance) {
    return 1.0f / (light_source.constant + light_source.linear * distance +
                   light_source.quadratic * distance * distance);
}

/**
 * Вычисляет интенсивность цвета от заданного источника света
 */
Color LightColor(const LightSource& source, const LightParameters& parameters,
                 const Material& material) {
    if (std::holds_alternative<AmbientLight>(source)) {
        const AmbientLight& light = std::get<AmbientLight>(source);
        return light.color * material.ambient * light.strength;
    }
    if (std::holds_alternative<DirectionalLight>(source)) {
        const DirectionalLight& light = std::get<DirectionalLight>(source);

        Vector light_direction = -TransformVector(light.direction, parameters.scene_to_camera);
        light_direction = glm::normalize(light_direction);

        float diff = glm::max(glm::dot(light_direction, parameters.normal), 0.0f);

        Vector view_direction = glm::normalize(-parameters.position);
        Vector mid_vec = glm::normalize(view_direction + light_direction);
        float spec =
            glm::pow(glm::max(glm::dot(mid_vec, parameters.normal), 0.0f), material.shininess);

        Color diffuse = material.diffuse * diff;
        Color specular = material.specular * spec;
        return (diffuse + specular) * light.color * light.strength;
    }
    if (std::holds_alternative<PointLight>(source)) {
        const PointLight& light = std::get<PointLight>(source);

        Point light_position = TransformPoint(light.position, parameters.scene_to_camera);
        Vector light_direction = (light_position - parameters.position);

        float distance = glm::length(light_direction);
        light_direction = glm::normalize(light_direction);

        float distance_strength = DistantStrength(light, distance);

        float diff = glm::max(glm::dot(light_direction, parameters.normal), 0.0f);
        Vector view_direction = glm::normalize(-parameters.position);
        Vector mid_vec = glm::normalize(view_direction + light_direction);
        float spec =
            glm::pow(glm::max(glm::dot(mid_vec, parameters.normal), 0.0f), material.shininess);

        Color diffuse = material.diffuse * diff;
        Color specular = material.specular * spec;
        return (diffuse + specular) * light.color * light.strength * distance_strength;
    }
    if (std::holds_alternative<SpotLight>(source)) {
        const SpotLight& light = std::get<SpotLight>(source);

        Point light_position = TransformPoint(light.position, parameters.scene_to_camera);
        Vector light_direction = (light_position - parameters.position);

        float distance = glm::length(light_direction);
        light_direction = glm::normalize(light_direction);

        Vector beam_direction = TransformVector(light.direction, parameters.scene_to_camera);
        beam_direction = glm::normalize(beam_direction);

        float distance_strength =
            glm::pow(glm::max(-glm::dot(beam_direction, light_direction), 0.0f), light.exponent) *
            DistantStrength(light, distance);

        float diff = glm::max(glm::dot(light_direction, parameters.normal), 0.0f);
        Vector view_direction = glm::normalize(-parameters.position);
        Vector mid_vec = glm::normalize(view_direction + light_direction);
        float spec =
            glm::pow(glm::max(glm::dot(mid_vec, parameters.normal), 0.0f), material.shininess);

        Color diffuse = material.diffuse * diff;
        Color specular = material.specular * spec;
        return (diffuse + specular) * light.color * light.strength * distance_strength;
    }
    {
        assert(false and "LightColor: неизвестный тип источника света");
    }
    return Vector{0};
}

}  // namespace

Image Renderer::Render(const Scene& scene, const Scene::CameraId camera_id, Image&& image,
                       const RenderFlags flags) {
    if (image.GetWidth() != 0 and image.GetHeight() != 0) {
        {
            assert((image.GetWidth() != 0) and
                   "Render: ширина переданного изображения не может быть 0");
            assert((image.GetHeight() != 0) and
                   "Render: высота переданного изображения не может быть 0");

            assert((scene.HasCamera(camera_id)) and "Render: камера должна принадлежать сцене");
        }
        flags_ = flags;
        UpdateInternalState(image.GetWidth(), image.GetHeight(),
                            scene.AccessCamera(camera_id).GetFocalLength(),
                            scene.AccessCamera(camera_id).GetFovX());
        parameters_.light_begin = scene.LightBegin();
        parameters_.light_end = scene.LightEnd();
        parameters_.scene_to_camera = scene.AccessCamera(camera_id).GetViewMatrix();
        const Object::FacetType* facets_storage = scene.AccessFacetsStorage();
        for (auto objects_it = scene.ObjectsBegin(); objects_it != scene.ObjectsEnd();
             ++objects_it) {
            Matrix object_to_camera = parameters_.scene_to_camera * objects_it->GetObjectMatrix();
            Matrix3 normal_to_camera = glm::transpose(glm::inverse(Matrix3{object_to_camera}));
            for (size_t triangle_index = 0; triangle_index < objects_it->Size(); ++triangle_index) {
                Triangle triangle = facets_storage[objects_it->Begin() + triangle_index];
                // нормализуем координаты и переводим в координаты камеры
                for (size_t i = 0; i < 3; ++i) {
                    triangle.vertices[i].point =
                        TransformPoint(triangle.vertices[i].point, object_to_camera);
                    triangle.vertices[i].normal = glm::normalize(
                        TransformVector(triangle.vertices[i].normal, normal_to_camera));
                }
                // находим нормаль к грани
                Vector triangle_normal =
                    glm::cross(triangle.vertices[1].point - triangle.vertices[0].point,
                               triangle.vertices[2].point - triangle.vertices[0].point);

                // отсечение по направлению грани
                const Material& material =
                    ResourcesManager::Get().AccessMaterial(triangle.material);
                if ((flags_ & DISABLE_BACKFACE_CULLING) == 0 and (not material.two_sided)) {
                    Vector camera_direction = -triangle.vertices[0].point;
                    if (glm::dot(triangle_normal, camera_direction) < 0.0f) {
                        continue;
                    }
                }

                // отсечение по пирамиде зрения
                Triangle clipped_triangles[63];
                size_t start;
                size_t size = ClipTriangle(triangle, clipped_triangles, &start);

                // отрисовка
                for (size_t i = 0; i < size; ++i) {
                    DrawTriangle(image, clipped_triangles[start + i]);
                }
            }
        }
    }
    return image;
}

void Renderer::DrawLine(Image& image, const Point& start, const Point& end) {
    // DDA-Line
    const size_t half_width = parameters_.width / 2;
    const size_t half_height = (z_buffer_.size() / parameters_.width) / 2;

    const int32_t x_start = std::round(start.x * parameters_.x_scale * half_width);
    const int32_t x_end = std::round(end.x * parameters_.x_scale * half_width);
    const int32_t y_start = std::round(start.y * parameters_.y_scale * half_height);
    const int32_t y_end = std::round(end.y * parameters_.y_scale * half_height);

    const uint32_t steps = std::max(std::abs(x_start - x_end), std::abs(y_start - y_end)) + 1;

    const Vector dp = (end - start) / static_cast<float>(steps);
    Point current_point = start;

    for (uint32_t current_step = 0; current_step <= steps; ++current_step, current_point += dp) {
        if (current_point.x < -1.0 or current_point.x > 1.0) {
            continue;
        }
        if (current_point.y < -1.0 or current_point.y > 1.0) {
            continue;
        }
        if (current_point.z < -1.0 or current_point.z > 1.0) {
            continue;
        }
        const int32_t screen_x = std::round(current_point.x * half_width) + half_width;
        const int32_t screen_y = half_height - std::round(current_point.y * half_height);
        if (screen_x < 0 or screen_x >= parameters_.width) {
            continue;
        }
        if (screen_y < 0 or screen_y >= z_buffer_.size() / parameters_.width) {
            continue;
        }
        current_point.z -= 10 * kEpsilon;  // более четкие границы при совмещении с растеризацией
        if (z_buffer_[screen_y * parameters_.width + screen_x] <= current_point.z) {
            continue;
        }
        z_buffer_[screen_y * parameters_.width + screen_x] = current_point.z;
        image.AccessPixel(screen_x, screen_y) = {.r = 0, .g = 255, .b = 0};
    }
}

void Renderer::DrawTriangle(Image& image, const Triangle& triangle) {

    Point4 clip_vertices[3];
    for (int i = 0; i < 3; ++i) {
        clip_vertices[i] = Point4{triangle.vertices[i].point, 1};
        clip_vertices[i] = parameters_.camera_to_clip * clip_vertices[i];
        {
            assert((glm::epsilonNotEqual(clip_vertices[i].w, 0.0f, kEpsilon)) and
                   "Renderer: после умножения на матрицу перхода в Clip "
                   "пространство координата w стала 0");
        }
        draw_parameters_.inv_w[i] = 1 / clip_vertices[i].w;
    }

    for (int i = 0; i < 3; ++i) {
        draw_parameters_.vertices[i] = clip_vertices[i] / clip_vertices[i].w;
    }

    if (flags_ & DRAW_EDGES) {
        DrawLine(image, draw_parameters_.vertices[0], draw_parameters_.vertices[1]);
        DrawLine(image, draw_parameters_.vertices[1], draw_parameters_.vertices[2]);
        DrawLine(image, draw_parameters_.vertices[2], draw_parameters_.vertices[0]);
    }

    if (flags_ & DRAW_FACETS) {
        int32_t width = parameters_.width;
        int32_t height = (z_buffer_.size() / parameters_.width);
        int32_t half_width = width / 2;
        int32_t half_height = height / 2;

        Vector scale_factor{half_width, half_height, 1};  // растяжение вдоль осей
        for (int i = 0; i < 3; ++i) {
            draw_parameters_.vertices[i] *= scale_factor;
        }

        // границы прямоугольника, содержащего треугольник (координаты экрана)
        float min_x =
            glm::min(draw_parameters_.vertices[0].x,
                     glm::min(draw_parameters_.vertices[1].x, draw_parameters_.vertices[2].x));
        float max_x =
            glm::max(draw_parameters_.vertices[0].x,
                     glm::max(draw_parameters_.vertices[1].x, draw_parameters_.vertices[2].x));
        float min_y =
            glm::min(draw_parameters_.vertices[0].y,
                     glm::min(draw_parameters_.vertices[1].y, draw_parameters_.vertices[2].y));
        float max_y =
            glm::max(draw_parameters_.vertices[0].y,
                     glm::max(draw_parameters_.vertices[1].y, draw_parameters_.vertices[2].y));

        // целочисленные границы, также обрезанные до границ экрана
        int32_t min_x_int = glm::round(min_x);
        min_x_int = glm::max(min_x_int, -half_width);

        int32_t max_x_int = glm::round(max_x);
        max_x_int = glm::min(max_x_int, width - half_width - 1);

        int32_t min_y_int = glm::round(min_y);
        min_y_int = glm::max(min_y_int, half_height - height + 1);

        int32_t max_y_int = glm::round(max_y);
        max_y_int = glm::min(max_y_int, half_height);

        ThreadPool& thread_pool = ThreadPool::Get();
        size_t threads = ThreadPool::GetThreadsCount();

        int32_t lines_per_thread = (max_y_int - min_y_int + 1) / threads + 1;
        int32_t current_line = min_y_int;
        for (size_t i = 0; i < threads and current_line <= max_y_int; ++i) {
            int32_t end_line = current_line + lines_per_thread - 1;
            end_line = std::min(end_line, max_y_int);
            thread_pool.Enqueue(
                [this, &image, &triangle, min_x_int, current_line, max_x_int, end_line]() {
                    TriangleRasterizationTask(image, triangle, min_x_int, current_line, max_x_int,
                                              end_line);
                });
            current_line = end_line + 1;
        }
        thread_pool.WaitAll();
    }
}

void Renderer::TriangleRasterizationTask(Image& image, const Triangle& triangle, const int32_t x0,
                                         const int32_t y0, const int32_t x1, const int32_t y1) {
    int32_t width = parameters_.width;
    int32_t height = (z_buffer_.size() / parameters_.width);
    int32_t half_width = width / 2;
    int32_t half_height = height / 2;
    const ResourcesManager& manager = ResourcesManager::Get();
    // перебор точек ограничивающего многоугольника
    for (int32_t y = y0; y <= y1; ++y) {
        for (int32_t x = x0; x <= x1; ++x) {
            Vector barycentric_coord =
                Barycentric(draw_parameters_.vertices[0], draw_parameters_.vertices[1],
                            draw_parameters_.vertices[2], Point2{x, y});
            // если хоть одна координата < 0, то точка вне треугольника. Вычисления
            // приближенные, из-за чего на краях могут появляться непрорисованне пиксели, для
            // чего используется менее строгое условие
            if (barycentric_coord.x < -2 * kEpsilon or barycentric_coord.y < -2 * kEpsilon or
                barycentric_coord.z < -2 * kEpsilon) {
                continue;
            }
            // точка внутри, проверка Z буффера
            float z = (draw_parameters_.vertices[0] * barycentric_coord.x +
                       draw_parameters_.vertices[1] * barycentric_coord.y +
                       draw_parameters_.vertices[2] * barycentric_coord.z)
                          .z;
            int32_t screen_x = x + half_width;
            int32_t screen_y = half_height - y;
            if (z_buffer_[screen_y * width + screen_x] < z) {
                continue;
            }
            z_buffer_[screen_y * width + screen_x] = z;
            float lambda = 1.0f / glm::dot(barycentric_coord, draw_parameters_.inv_w);
            Vector coefs = barycentric_coord * draw_parameters_.inv_w;
            Point2 uv_coordinates = (coefs[0] * triangle.vertices[0].uv_coordinates +
                                     coefs[1] * triangle.vertices[1].uv_coordinates +
                                     coefs[2] * triangle.vertices[2].uv_coordinates) *
                                    lambda;

            Color pixel_color = manager.GetPixelByUV(
                manager.AccessMaterial(triangle.material).texture, uv_coordinates);

            // вычисление света
            if (flags_ & ENABLE_LIGHT) {
                LightParameters light_parameters{.scene_to_camera = parameters_.scene_to_camera};
                light_parameters.position =
                    (coefs[0] * triangle.vertices[0].point + coefs[1] * triangle.vertices[1].point +
                     coefs[2] * triangle.vertices[2].point) *
                    lambda;
                light_parameters.normal = (coefs[0] * triangle.vertices[0].normal +
                                           coefs[1] * triangle.vertices[1].normal +
                                           coefs[2] * triangle.vertices[2].normal) *
                                          lambda;

                Color total_light_color{0, 0, 0};
                const Material& material = manager.AccessMaterial(triangle.material);
                for (auto it = parameters_.light_begin; it != parameters_.light_end; ++it) {
                    total_light_color += LightColor(*it, light_parameters, material);
                }
                pixel_color *= total_light_color;
            }

            pixel_color = glm::clamp(pixel_color, 0.0f, 1.0f);
            uint8_t r = pixel_color.r * 255;
            uint8_t g = pixel_color.g * 255;
            uint8_t b = pixel_color.b * 255;
            {
                assert((0 <= r and r <= 255) and
                       "TriangleRasterizationTask: компонента r вышла из диапазона");
                assert((0 <= g and g <= 255) and
                       "TriangleRasterizationTask: компонента g вышла из диапазона");
                assert((0 <= b and b <= 255) and
                       "TriangleRasterizationTask: компонента b вышла из диапазона");
            }
            image.AccessPixel(screen_x, screen_y) = {r, g, b};
        }
    }
}

void Renderer::UpdateInternalState(const size_t width, const size_t height,
                                   const float focal_length, const float fov_x) {
    {
        assert((width != 0) and
               "UpdateInternalState: ширина переданного изображения не может быть 0");
        assert((height != 0) and
               "UpdateInternalState: высота переданного изображения не может быть 0");

        assert((fov_x > 0.0 + kEpsilon) and "UpdateInternalState: fov_x должен быть больше 0.0");
        assert((fov_x < 360.0 - kEpsilon) and
               "UpdateInternalState: fov_x должен быть меньше 360.0");

        assert((focal_length >= 0.1) and
               "UpdateInternalState: focal_length должен быть не меньше 0.1");
        assert((focal_length <= 10.0) and
               "UpdateInternalState: focal_length должен быть не больше 10.0");
    }
    parameters_.width = width;
    const float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
    const float scale_factor = focal_length * std::tan(glm::radians(fov_x) / 2);

    const float fov_y = 2.0f * std::atan2(std::tan(glm::radians(fov_x) / 2), aspect_ratio);

    parameters_.x_scale = scale_factor;
    parameters_.y_scale = aspect_ratio / scale_factor;

    parameters_.camera_to_clip = glm::infinitePerspective(fov_y, aspect_ratio, focal_length);
    z_buffer_.assign(width * height, std::numeric_limits<float>::infinity());

    // Плоскости пирамиды зрения
    // Порядок: ближняя, левая, правая, нижняя, верхняя
    parameters_.frustum_planes[0] = {0, 0, -1, -focal_length};
    parameters_.frustum_planes[1] = {glm::normalize(Vector{focal_length, 0, -1}), 0};
    parameters_.frustum_planes[2] = {glm::normalize(Vector{-focal_length, 0, -1}), 0};
    parameters_.frustum_planes[3] = {glm::normalize(Vector{0, focal_length, -aspect_ratio}), 0};
    parameters_.frustum_planes[4] = {glm::normalize(Vector{0, -focal_length, -aspect_ratio}), 0};
}

size_t Renderer::ClipTriangle(const Triangle& triangle, Triangle* result, size_t* start) {
    {
        assert(result and "ClipTriangle: result не должен быть nullptr");
        assert(start and "ClipTriangle: start не должен быть nullptr");
    }
    /*
     * Алгоритм последовательно обрезает треугольники относительно 5 границ пирамиды зрения
     * Результат каждой хранится в result подряд, рузультаты обрезок хранятся подряд
     * Изначально треугольник 1, на каждой итерации число треугольников не более чем удваивается,
     * поэтому для хранения всех промежуточных результатов и итога нужно не более 1 + 2 + ... + 2^5
     * = 63 ячейки
     *
     * Схема раслопожения данных (массив сверху-вниз, слева-направо)
     * #        <- начало, только изначальный треугольник
     * ##       <- после 1 итерации
     * ###
     * ....
     * ######## <- после 5 итераций, итог
     *
     * iteration_start указывает на начало уровня в result, iteration_start - размер текущего
     * уровня, next_iteration_size - размер следующего уровня
     *
     */
    size_t iteration_start = 0;
    size_t iteration_size = 1;
    result[0] = triangle;
    for (int iteration = 0; iteration < 5; ++iteration) {
        size_t next_iteration_size = 0;
        for (size_t i = 0; i < iteration_size; ++i) {
            next_iteration_size += ClipTriangleAganistPlane(
                result[iteration_start + i], parameters_.frustum_planes[iteration],
                result + iteration_start + iteration_size + next_iteration_size);
        }
        iteration_start = iteration_start + iteration_size;
        iteration_size = next_iteration_size;
    }
    *start = iteration_start;
    {
        assert((iteration_start + iteration_size < 64) and
               "ClipTriangle: должно использоваться не более 63 элементов массива");
    }
    return iteration_size;
}

}  // namespace renderer
