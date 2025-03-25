#include "renderer/renderer.hpp"

#include <glm/geometric.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace renderer {

namespace {
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
        assert((glm::epsilonNotEqual(denominator, 0.0f, glm::epsilon<float>())) and
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

}  // namespace

Image Renderer::Render(const Scene& scene, const Scene::CameraId camera_id, Image&& image) {
    if (image.GetWidth() != 0 and image.GetHeight() != 0) {
        {
            assert((image.GetWidth() != 0) and
                   "Render: ширина переданного изображения не может быть 0");
            assert((image.GetHeight() != 0) and
                   "Render: высота переданного изображения не может быть 0");

            assert((scene.HasCamera(camera_id)) and "Render: камера должна принадлежать сцене");
        }
        UpdateInternalState(image.GetWidth(), image.GetHeight(),
                            scene.AccessCamera(camera_id).GetFocalLength(),
                            scene.AccessCamera(camera_id).GetFovX());
        Matrix camera_view_matrix = scene.AccessCamera(camera_id).GetViewMatrix();
        const Object::FacetType* facets_storage = scene.AccessFacetsStorage();
        for (auto objects_it = scene.ObjectsBegin(); objects_it != scene.ObjectsEnd();
             ++objects_it) {
            Matrix object_to_camera = camera_view_matrix * objects_it->GetObjectMatrix();

            for (size_t triangle_index = 0; triangle_index < objects_it->Size(); ++triangle_index) {
                Triangle triangle = facets_storage[objects_it->Begin() + triangle_index];
                // нормализуем координаты и переводим в координаты камеры
                for (size_t i = 0; i < 3; ++i) {
                    Point4 new_triangle_point{triangle.vertices[i].point, 1};
                    new_triangle_point = object_to_camera * new_triangle_point;
                    {
                        assert((glm::epsilonNotEqual(new_triangle_point.w, 0.0f,
                                                     glm::epsilon<float>())) and
                               "Render: после умножения на матрицу перехода в пространство камеры "
                               "координата w стала 0");
                    }
                    new_triangle_point /= new_triangle_point.w;
                    triangle.vertices[i].point = new_triangle_point;
                }
                // находим нормаль к грани
                Vector triangle_normal =
                    glm::cross(triangle.vertices[1].point - triangle.vertices[0].point,
                               triangle.vertices[2].point - triangle.vertices[0].point);

                // отсечение по направлению грани
                Vector camera_direction = -triangle.vertices[0].point;
                if (glm::dot(triangle_normal, camera_direction) < 0.0f) {
                    continue;
                }

                // отсечение по пирамиде зрения
                Triangle clipped_triangles[63];
                size_t start;
                size_t size = ClipTriangle(triangle, clipped_triangles, &start);

                // пересчет в clip пространство
                for (size_t triangle_index = 0; triangle_index < size; ++triangle_index) {
                    for (size_t i = 0; i < 3; ++i) {
                        Point4 new_triangle_point{
                            clipped_triangles[start + triangle_index].vertices[i].point, 1};
                        new_triangle_point = parameters_.camera_to_clip * new_triangle_point;
                        {
                            assert((glm::epsilonNotEqual(new_triangle_point.w, 0.0f,
                                                         glm::epsilon<float>())) and
                                   "Renderer: после умножения на матрицу перхода в Clip "
                                   "пространство координата w стала 0");
                        }
                        new_triangle_point /= new_triangle_point.w;
                        clipped_triangles[start + triangle_index].vertices[i].point =
                            new_triangle_point;
                    }
                }

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
        if (z_buffer_[screen_y * parameters_.width + screen_x] < current_point.z) {
            continue;
        }
        z_buffer_[screen_y * parameters_.width + screen_x] = current_point.z;
        image.AccessPixel(screen_x, screen_y) = {.r = 255, .g = 255, .b = 255};
    }
}

void Renderer::DrawTriangle(Image& image, const Triangle& triangle) {
    DrawLine(image, triangle.vertices[0].point, triangle.vertices[1].point);
    DrawLine(image, triangle.vertices[1].point, triangle.vertices[2].point);
    DrawLine(image, triangle.vertices[2].point, triangle.vertices[0].point);
}

void Renderer::UpdateInternalState(const size_t width, const size_t height,
                                   const float focal_length, const float fov_x) {
    {
        assert((width != 0) and
               "UpdateInternalState: ширина переданного изображения не может быть 0");
        assert((height != 0) and
               "UpdateInternalState: высота переданного изображения не может быть 0");

        assert((fov_x > 0.0 + glm::epsilon<float>()) and
               "UpdateInternalState: fov_x должен быть больше 0.0");
        assert((fov_x < 360.0 - glm::epsilon<float>()) and
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
