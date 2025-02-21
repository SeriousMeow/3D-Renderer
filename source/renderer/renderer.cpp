#include "renderer/renderer.hpp"

#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace renderer {

Image Renderer::Render(const Scene& scene, const Scene::CameraId camera_id, Image&& image) {
    {
        assert((image.GetWidth() != 0) and
               "Render: ширина переданного изображения не может быть 0");
        assert((image.GetHeight() != 0) and
               "Render: высота переданного изображения не может быть 0");

        assert((scene.HasCamera(camera_id)) and "Render: камера должна принадлежать сцене");
    }
    if (image.GetWidth() != 0 and image.GetHeight() != 0) {

        UpdateInternalState(image.GetWidth(), image.GetHeight(),
                            scene.AccessCamera(camera_id).GetFocalLenght(),
                            scene.AccessCamera(camera_id).GetFovX());
        for (Scene::ObjectsIterator objects_it = scene.ObjectsBegin();
             objects_it != scene.ObjectsEnd(); ++objects_it) {
            Matrix object_to_camera =
                scene.AccessCamera(camera_id).AccessMatrix() * objects_it->object_to_scene;
            const Object& object = (*objects_it).object;

            for (Object::ConstIterator triangle_it = object.Begin(); triangle_it != object.End();
                 ++triangle_it) {
                Triangle triangle = *triangle_it;
                Point4 vertices[3];
                // нормализуем координаты и переводим в координаты камеры
                for (size_t i = 0; i < 3; ++i) {
                    vertices[i] = Point4{triangle.vertices[i].point, 1.0f};
                    vertices[i] = object_to_camera * vertices[i];
                    {
                        assert(
                            (glm::epsilonNotEqual(vertices[i].w, 0.0f, glm::epsilon<float>())) and
                            "Render: после умножения на матрицу перехода в пространство камеры "
                            "координата w стала 0");
                    }
                    vertices[i] /= vertices[i].w;
                }
                // находим нормаль к грани
                Point4 triangle_center = (vertices[0] + vertices[1] + vertices[2]) / 3.0f;
                Vector triangle_normal = glm::cross(Vector{vertices[1] - vertices[0]},
                                                    Vector{vertices[2] - vertices[0]});

                // проверяем, что грань направлена в сторону камеры
                Vector camera_direction = -Vector{triangle_center};
                if (glm::dot(triangle_normal, camera_direction) < 0.0f) {
                    continue;
                }
                for (size_t i = 0; i < 3; ++i) {
                    vertices[i] = parameters_.camera_to_clip * vertices[i];
                    assert((glm::epsilonNotEqual(vertices[i].w, 0.0f, glm::epsilon<float>())) and
                           "Renderer: после умножения на матрицу перхода в Clip пространство "
                           "координата w стала 0");
                    vertices[i] /= vertices[i].w;
                }
                DrawLine(image, Point{vertices[0]}, Point{vertices[1]});
                DrawLine(image, Point{vertices[1]}, Point{vertices[2]});
                DrawLine(image, Point{vertices[2]}, Point{vertices[0]});
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
}
}  // namespace renderer
