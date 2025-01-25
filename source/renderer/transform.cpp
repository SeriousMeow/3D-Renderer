#include "renderer/transform.hpp"

#include <glm/ext/matrix_transform.hpp>

renderer::Matrix renderer::transforms::Translate(const renderer::Vector& direction) {
    return glm::translate(kNoTransforms, direction);
}

renderer::Matrix renderer::transforms::Scale(const float x_scale_factor, const float y_scale_factor,
                                             const float z_scale_factor) {
    return glm::scale(kNoTransforms, Vector{x_scale_factor, y_scale_factor, z_scale_factor});
}

renderer::Matrix renderer::transforms::ScaleUniform(const float scale_factor) {
    return Scale(scale_factor, scale_factor, scale_factor);
}

renderer::Matrix renderer::transforms::RotateDegrees(const float angle, const Vector& axis) {
    return glm::rotate(kNoTransforms, glm::radians(angle), axis);
}

renderer::Matrix renderer::transforms::RotateRadians(const float angle, const Vector& axis) {
    return glm::rotate(kNoTransforms, angle, axis);
}

renderer::Matrix renderer::transforms::RotateAboutAxisX(const float angle) {
    return RotateDegrees(angle, Vector{1, 0, 0});
}

renderer::Matrix renderer::transforms::RotateAboutAxisY(const float angle) {
    return RotateDegrees(angle, Vector{0, 1, 0});
}

renderer::Matrix renderer::transforms::RotateAboutAxisZ(const float angle) {
    return RotateDegrees(angle, Vector{0, 0, 1});
}

renderer::Matrix renderer::transforms::CameraLookAtPoint(const Point& camera_position,
                                                         const Point& center) {
    return glm::lookAt(camera_position, center, Vector{0, 0, 1});
}

renderer::Matrix renderer::transforms::CameraLookAtDirection(const Point& camera_position,
                                                             const Vector& direction) {
    Point center = camera_position + direction;
    return glm::lookAt(camera_position, center, Vector{0, 0, 1});
}
