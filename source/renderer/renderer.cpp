#include "renderer/renderer.hpp"

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
    z_buffer_.assign(width_ * height_, -std::numeric_limits<float>::infinity());
    for (Camera::Iterator object_camera_space_it = camera_->Begin();
         object_camera_space_it != camera_->End(); ++object_camera_space_it) {
        TransformMatrix object_to_clip_space_matrix =
            (*object_camera_space_it).object_to_camera_matrix * camera_to_clip_matrix_;

        Object* object_ptr = (*object_camera_space_it).object_ptr;
        assert(object_ptr and "Renderer: получен nullptr указатель на object от camera");

        for (Object::Iterator triangle_it = object_ptr->Begin(); triangle_it != object_ptr->End();
             ++triangle_it) {

            Triangle triangle = *triangle_it;
            Point new_vertices[3];
            for (size_t i = 0; i < 3; ++i) {
                new_vertices[i] = object_to_clip_space_matrix * triangle.vertices[i];
                assert((std::abs(new_vertices[i][3]) > 1e-5) and
                       "Renderer: точка для отрисовки имеет слишком малую координату w");
                new_vertices[i] /= new_vertices[i][3];
            }
            DrawLine(image, new_vertices[0], new_vertices[1]);
            DrawLine(image, new_vertices[1], new_vertices[2]);
            DrawLine(image, new_vertices[2], new_vertices[0]);
        }
    }
}

void renderer::Renderer::DrawLine(Image& image, const Point& start, const Point& end) {
    // DDA-Line
    const size_t half_width = width_ / 2;
    const size_t half_height = height_ / 2;

    const int32_t x_start = std::round(start[0] * x_scale_factor_ * half_width);
    const int32_t x_end = std::round(end[0] * x_scale_factor_ * half_width);
    const int32_t y_start = std::round(start[1] * y_scale_factor_ * half_height);
    const int32_t y_end = std::round(end[1] * y_scale_factor_ * half_height);

    const uint32_t steps = std::max(std::abs(x_start - x_end), std::abs(y_start - y_end)) + 1;
    const float dx = (end[0] - start[0]) / steps;
    const float dy = (end[1] - start[1]) / steps;
    const float dz = (end[2] - start[2]) / steps;

    float x = start[0];
    float y = start[1];
    float z = start[2];

    for (uint32_t current_step = 0; current_step <= steps;
         ++current_step, x += dx, y += dy, z += dz) {
        if (x < -1.0 or x > 1.0) {
            continue;
        }
        if (y < -1.0 or y > 1.0) {
            continue;
        }
        if (z < -1.0 or z > 1.0) {
            continue;
        }
        const int32_t screen_x = std::round(x * half_width) + half_width;
        const int32_t screen_y = half_height - std::round(y * half_height);
        if (screen_x < 0 or screen_x >= width_) {
            continue;
        }
        if (screen_y < 0 or screen_y >= height_) {
            continue;
        }
        if (z_buffer_[screen_y * width_ + screen_x] > z) {
            continue;
        }
        z_buffer_[screen_y * width_ + screen_x] = z;
        image.GetImageReference()[screen_y * width_ + screen_x] =
            Image::Pixel{.r = 1.0, .g = 1.0, .b = 1.0};
    }
}

void renderer::Renderer::Init(const size_t width, const size_t height,
                              const float near_plane_distance, const float fov) {
    width_ = width;
    height_ = height;
    const float scale_factor = near_plane_distance * std::tan(fov / 2);
    const float aspect_ratio = static_cast<float>(height) / static_cast<float>(width);

    x_scale_factor_ = scale_factor;
    y_scale_factor_ = aspect_ratio * scale_factor;

    const float left_bound = -x_scale_factor_;
    const float right_bound = x_scale_factor_;
    const float top_bound = y_scale_factor_;
    const float bottom_bound = -x_scale_factor_;

    camera_to_clip_matrix_ = TransformMatrix{
        {(2 * near_plane_distance) / (right_bound - left_bound), 0.0,
         (right_bound + left_bound) / (right_bound - left_bound), 0.0},
        {0.0, (2 * near_plane_distance) / (top_bound - bottom_bound),
         (top_bound + bottom_bound) / (top_bound - bottom_bound), 0.0},
        {0.0, 0.0, -1.0, -2 * near_plane_distance},
        {0.0, 0.0, -1.0, 0.0},
    };
}
