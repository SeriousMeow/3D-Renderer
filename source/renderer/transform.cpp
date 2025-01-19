#include "renderer/transform.hpp"

#include <glm/ext/matrix_transform.hpp>

renderer::TransformMatrix renderer::transfroms::Translate(const renderer::Vector& direction) {
    return glm::translate(kNoTransforms, direction);
}

renderer::TransformMatrix renderer::transfroms::Scale(const float x_scale_factor,
                                                      const float y_scale_factor,
                                                      const float z_scale_factor) {
    return glm::scale(kNoTransforms, Vector{x_scale_factor, y_scale_factor, z_scale_factor});
}

renderer::TransformMatrix renderer::transfroms::ScaleUniform(const float scale_factor) {
    return Scale(scale_factor, scale_factor, scale_factor);
}

renderer::TransformMatrix renderer::transfroms::RotateDegrees(const float angle,
                                                              const Vector& axis) {
    return glm::rotate(kNoTransforms, glm::radians(angle), axis);
}

renderer::TransformMatrix renderer::transfroms::RotateRadians(const float angle,
                                                              const Vector& axis) {
    return glm::rotate(kNoTransforms, angle, axis);
}

renderer::TransformMatrix renderer::transfroms::RotateAboutAxisX(const float angle) {
    return RotateDegrees(angle, Vector{1, 0, 0});
}

renderer::TransformMatrix renderer::transfroms::RotateAboutAxisY(const float angle) {
    return RotateDegrees(angle, Vector{0, 1, 0});
}

renderer::TransformMatrix renderer::transfroms::RotateAboutAxisZ(const float angle) {
    return RotateDegrees(angle, Vector{0, 0, 1});
}

renderer::TransformMatrix renderer::transfroms::CameraLookAtPoint(const Point& camera_position,
                                                                  const Point& center) {
    return glm::lookAt(camera_position, center, Vector{0, 0, 1});
}

renderer::TransformMatrix renderer::transfroms::CameraLookAtDirection(const Point& camera_position,
                                                                      const Vector& direction) {
    Point center = camera_position + direction;
    return glm::lookAt(camera_position, center, Vector{0, 0, 1});
}
