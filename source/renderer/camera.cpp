#include "camera.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>

namespace renderer {

Camera::Camera(const Point& position, const float yaw, const float pitch, const float fov_x,
               const float focal_length)
    : position_{position}, yaw_{yaw}, pitch_{pitch}, fov_x_{fov_x}, focal_length_{focal_length} {
    {
        assert((pitch >= -90.0) and "Camera: pitch должен быть не меньше -90.0");
        assert((pitch <= 90.0) and "Camera: pitch должен быть не больше 90.0");

        assert((fov_x > 0.0 + glm::epsilon<float>()) and "Camera: fov_x должен быть больше 0.0");
        assert((fov_x < 360.0 - glm::epsilon<float>()) and
               "Camera: fov_x должен быть меньше 360.0");

        assert((focal_length >= 0.1) and "Camera: focal_length должен быть не меньше 0.1");
        assert((focal_length <= 10.0) and "Camera: focal_length должен быть не больше 10.0");
    }
    UpdateVectors();
}

Matrix Camera::GetViewMatrix() const {
    return glm::lookAt(position_, position_ + front_, up_);
}

Point Camera::GetPosition() const {
    return position_;
}

void Camera::SetPosition(const Point& new_position) {
    position_ = new_position;
}

float Camera::GetYaw() const {
    return yaw_;
}

void Camera::SetYaw(const float new_yaw) {
    yaw_ = new_yaw;
    UpdateVectors();
}

float Camera::GetPitch() const {
    return pitch_;
}

void Camera::SetPitch(const float new_pitch) {
    {
        assert((new_pitch >= -90.0) and "SetPitch: new_pitch должен быть не меньше -90.0");
        assert((new_pitch <= 90.0) and "SetPitch: new_pitch должен быть не больше 90.0");
    }
    pitch_ = new_pitch;
    UpdateVectors();
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

float Camera::GetFocalLength() const {
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

void Camera::UpdateVectors() {
    Vector new_front;
    new_front.x = glm::cos(glm::radians(yaw_)) * glm::cos(glm::radians(pitch_));
    new_front.y = glm::sin(glm::radians(yaw_)) * glm::cos(glm::radians(pitch_));
    new_front.z = glm::sin(glm::radians(pitch_));
    front_ = glm::normalize(new_front);
    right_ = glm::normalize(glm::cross(front_, kWorldUp));
    up_ = glm::normalize(glm::cross(right_, front_));
}

}  // namespace renderer
