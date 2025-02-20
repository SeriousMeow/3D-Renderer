#include "camera.hpp"

#include "glm/ext/scalar_constants.hpp"

namespace renderer {

Camera::Camera(const Matrix& scene_to_camera, const float fov_x, const float focal_length)
    : scene_to_camera_{scene_to_camera}, fov_x_{fov_x}, focal_length_{focal_length} {
    {
        assert((fov_x > 0.0 + glm::epsilon<float>()) and "Camera: fov_x должен быть больше 0.0");
        assert((fov_x < 360.0 - glm::epsilon<float>()) and
               "Camera: fov_x должен быть меньше 360.0");

        assert((focal_length >= 0.1) and "Camera: focal_length должен быть не меньше 0.1");
        assert((focal_length <= 10.0) and "Camera: focal_length должен быть не больше 10.0");
    }
}

Matrix& Camera::AccessMatrix() {
    return scene_to_camera_;
}

const Matrix& Camera::AccessMatrix() const {
    return scene_to_camera_;
}

float Camera::GetFovX() const {
    return fov_x_;
}

void Camera::SetFovX(const float new_fov_x) {
    {
        assert((new_fov_x > 0.0 + glm::epsilon<float>()) and
               "SetFovX: new_fov_x должен быть больше 0.0");
        assert((new_fov_x < 360.0 - glm::epsilon<float>()) and
               "SetFovX: new_fov_x должен быть меньше 360.0");
    }
    fov_x_ = new_fov_x;
}

float Camera::GetFocalLenght() const {
    return focal_length_;
}

void Camera::SetFocalLength(const float new_focal_length) {
    {
        assert((new_focal_length >= 0.1) and
               "SetFocalLength: new_focal_length должен быть не меньше 0.1");
        assert((new_focal_length <= 10.0) and
               "SetFocalLength: new_focal_length должен быть не больше 10.0");
    }
    focal_length_ = new_focal_length;
}

}  // namespace renderer
