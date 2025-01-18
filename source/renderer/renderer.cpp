#include "renderer/renderer.hpp"

#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>

renderer::Renderer::Renderer(renderer::Camera* camera) : camera_{camera} {
    assert(camera and "Renderer: camera не может быть nullptr");
}

void renderer::Renderer::Render(renderer::Image& image) {
    {
        {
            assert((image.GetWidth() == width_) and
                   "Renderer: ширина переданного изображения не совпадает с переданным в init");
            assert((image.GetHeight() == height_) and
                   "Renderer: высота переданного изображения не совпадает с переданным в init");
        }
        {
            assert((width_ != 0) and "Render: ширина не может быть 0");
            assert((height_ != 0) and "Render: высота не может быть 0");
        }
    }
    z_buffer_.assign(width_ * height_, std::numeric_limits<float>::infinity());
    for (Camera::Iterator object_camera_space_it = camera_->Begin();
         object_camera_space_it != camera_->End(); ++object_camera_space_it) {
        TransformMatrix object_to_camera = (*object_camera_space_it).object_to_camera_matrix;

        Object* object_ptr = (*object_camera_space_it).object_ptr;
        assert(object_ptr and "Renderer: получен nullptr указатель на object от camera");

        for (Object::Iterator triangle_it = object_ptr->Begin(); triangle_it != object_ptr->End();
             ++triangle_it) {

            Triangle triangle = *triangle_it;
            // нормализуем координаты и переводим в координаты камеры
            for (size_t i = 0; i < 3; ++i) {

                assert(
                    (glm::epsilonNotEqual(triangle.vertices[i].w, 0.0f, glm::epsilon<float>()) and
                     "Renderer: точка в треугольнике имеет координату w, равную 0"));
                triangle.vertices[i] = object_to_camera * triangle.vertices[i];
                triangle.vertices[i] /= triangle.vertices[i].w;
            }
            Point triangle_center =
                (triangle.vertices[0] + triangle.vertices[1] + triangle.vertices[2]) / 3.0f;
            glm::vec3 triangle_normal =
                glm::cross(glm::vec3{triangle.vertices[1] - triangle.vertices[0]},
                           glm::vec3{triangle.vertices[2] - triangle.vertices[0]});
            triangle_normal = glm::normalize(triangle_normal);
            glm::vec3 camera_direction = -glm::vec3{triangle_center};
            if (glm::dot(triangle_normal, camera_direction) < 0.0f) {
                continue;
            }
            Point new_vertices[3];
            for (size_t i = 0; i < 3; ++i) {
                new_vertices[i] = camera_to_clip_matrix_ * triangle.vertices[i];
                assert((std::abs(new_vertices[i].w) > 1e-5) and
                       "Renderer: точка для отрисовки имеет слишком малую координату w");
                new_vertices[i] /= new_vertices[i].w;
            }
            DrawLine(image, new_vertices[0], new_vertices[1]);
            DrawLine(image, new_vertices[1], new_vertices[2]);
            DrawLine(image, new_vertices[2], new_vertices[0]);

            Point triangle_normal_start = triangle_center;
            Point triangle_normal_end = triangle_center + glm::vec4{triangle_normal, 0};
            Point triangle_normal_start_new = camera_to_clip_matrix_ * triangle_normal_start;
            triangle_normal_start_new /= triangle_normal_start_new.w;
            Point triangle_normal_end_new = camera_to_clip_matrix_ * triangle_normal_end;
            triangle_normal_end_new /= triangle_normal_end_new.w;
            // DrawLine(image, triangle_normal_start_new, triangle_normal_end_new);
        }
    }
}

void renderer::Renderer::DrawLine(Image& image, const Point& start, const Point& end) {
    // DDA-Line
    const size_t half_width = width_ / 2;
    const size_t half_height = height_ / 2;

    const int32_t x_start = std::round(start.x * x_scale_factor_ * half_width);
    const int32_t x_end = std::round(end.x * x_scale_factor_ * half_width);
    const int32_t y_start = std::round(start.y * y_scale_factor_ * half_height);
    const int32_t y_end = std::round(end.y * y_scale_factor_ * half_height);

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
        if (screen_x < 0 or screen_x >= width_) {
            continue;
        }
        if (screen_y < 0 or screen_y >= height_) {
            continue;
        }
        if (z_buffer_[screen_y * width_ + screen_x] < current_point.z) {
            continue;
        }
        z_buffer_[screen_y * width_ + screen_x] = current_point.z;
        image.GetImageReference()[screen_y * width_ + screen_x] =
            Image::Pixel{.r = 1.0, .g = 1.0, .b = 1.0};
    }
}

void renderer::Renderer::Init(const size_t width, const size_t height,
                              const float near_plane_distance, const float fov_x) {
    {
        assert((fov_x > 0.0f + glm::epsilon<float>()) and "Init: fov_x должен быть больше 0.0");
        assert((fov_x < 360.0f - glm::epsilon<float>()) and "Init: fov_x должен быть меньше 360.0");
    }
    width_ = width;
    height_ = height;

    const float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
    const float scale_factor = near_plane_distance * std::tan(glm::radians(fov_x) / 2);

    const float fov_y = 2.0f * std::atan2(std::tan(glm::radians(fov_x) / 2), aspect_ratio);

    x_scale_factor_ = scale_factor;
    y_scale_factor_ = aspect_ratio / scale_factor;

    camera_to_clip_matrix_ = glm::infinitePerspective(fov_y, aspect_ratio, near_plane_distance);
}
