#include "renderer/transform.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/epsilon.hpp>

namespace {
const float kEPS = glm::epsilon<float>();
};

renderer::TransformMatrix renderer::transfroms::Translate(const renderer::Vector& direction) {
    {
        assert((glm::epsilonEqual(direction.w, 0.0f, kEPS)) and
               "ShiftByDirection: координата w вектора должна быть 0.0");
    }
    return glm::translate(kNoTransforms, glm::vec3{direction});
}

renderer::TransformMatrix renderer::transfroms::Scale(const float x_scale_factor,
                                                      const float y_scale_factor,
                                                      const float z_scale_factor) {
    return glm::scale(kNoTransforms, glm::vec3{x_scale_factor, y_scale_factor, z_scale_factor});
}

renderer::TransformMatrix renderer::transfroms::ScaleUniform(const float scale_factor) {
    return Scale(scale_factor, scale_factor, scale_factor);
}

renderer::TransformMatrix renderer::transfroms::RotateDegrees(const float angle,
                                                              const Vector& axis) {
    {
        assert((glm::epsilonEqual(axis.w, 0.0f, kEPS)) and
               "RotateDegrees: координата w вектора должна быть 0.0");
    }
    return glm::rotate(kNoTransforms, glm::radians(angle), glm::vec3{axis});
}

renderer::TransformMatrix renderer::transfroms::RotateRadians(const float angle,
                                                              const Vector& axis) {
    {
        assert((glm::epsilonEqual(axis.w, 0.0f, kEPS)) and
               "RotateRadians: координата w вектора должна быть 0.0");
    }
    return glm::rotate(kNoTransforms, angle, glm::vec3{axis});
}

renderer::TransformMatrix renderer::transfroms::RotateAboutAxisX(const float angle) {
    return RotateDegrees(angle, Vector{1, 0, 0, 0});
}

renderer::TransformMatrix renderer::transfroms::RotateAboutAxisY(const float angle) {
    return RotateDegrees(angle, Vector{0, 1, 0, 0});
}

renderer::TransformMatrix renderer::transfroms::RotateAboutAxisZ(const float angle) {
    return RotateDegrees(angle, Vector{0, 0, 1, 0});
}

renderer::TransformMatrix renderer::transfroms::CameraLookAtPoint(const Point& camera_position,
                                                                  const Point& center) {
    {
        assert((glm::epsilonNotEqual(camera_position.w, 0.0f, kEPS)) and
               "CameraLookAtPoint: координата w точки camera_position не должа быть 0");
        assert((glm::epsilonNotEqual(center.w, 0.0f, kEPS)) and
               "CameraLookAtPoint: координата w точки center не должа быть 0");
    }
    return glm::lookAt(glm::vec3{camera_position / camera_position.w}, glm::vec3{center / center.w},
                       glm::vec3{0, 0, 1});
}

renderer::TransformMatrix renderer::transfroms::CameraLookAtDirection(const Point& camera_position,
                                                                      const Vector& direction) {
    {
        assert((glm::epsilonNotEqual(camera_position.w, 0.0f, kEPS)) and
               "CameraLookAtDirection: координата w точки camera_position не должна быть 0");
        assert((glm::epsilonEqual(direction.w, 0.0f, kEPS)) and
               "CameraLookAtDirection: координата w вектора direction должна быть 0");
    }
    Point center = camera_position / camera_position.w + direction;
    return glm::lookAt(glm::vec3{camera_position / camera_position.w}, glm::vec3(center),
                       glm::vec3{0, 0, 1});
}
