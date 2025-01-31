#include "renderer/transform.hpp"

#include <glm/ext/matrix_transform.hpp>

namespace renderer::transforms {

Matrix Translate(const renderer::Vector& direction) {
    return glm::translate(kNoTransforms, direction);
}

Matrix Scale(const float x_scale_factor, const float y_scale_factor, const float z_scale_factor) {
    return glm::scale(kNoTransforms, Vector{x_scale_factor, y_scale_factor, z_scale_factor});
}

Matrix ScaleUniform(const float scale_factor) {
    return Scale(scale_factor, scale_factor, scale_factor);
}

Matrix RotateDegrees(const float angle, const Vector& axis) {
    return glm::rotate(kNoTransforms, glm::radians(angle), axis);
}

Matrix RotateRadians(const float angle, const Vector& axis) {
    return glm::rotate(kNoTransforms, angle, axis);
}

Matrix RotateAboutAxisX(const float angle) {
    return RotateDegrees(angle, Vector{1, 0, 0});
}

Matrix RotateAboutAxisY(const float angle) {
    return RotateDegrees(angle, Vector{0, 1, 0});
}

Matrix RotateAboutAxisZ(const float angle) {
    return RotateDegrees(angle, Vector{0, 0, 1});
}

Matrix CameraLookAtPoint(const Point& camera_position, const Point& center) {
    return glm::lookAt(camera_position, center, Vector{0, 0, 1});
}

Matrix CameraLookAtDirection(const Point& camera_position, const Vector& direction) {
    Point center = camera_position + direction;
    return glm::lookAt(camera_position, center, Vector{0, 0, 1});
}
}  // namespace renderer::transforms
