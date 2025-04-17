#include "scene_object.hpp"

#include <glm/ext/matrix_transform.hpp>

namespace renderer {

SceneObject::SceneObject(const size_t begin, const size_t size, const Point& position,
                         const float x_angle, const float y_angle, const float z_angle,
                         const float scale)
    : position_{position},
      x_angle_{x_angle},
      y_angle_{y_angle},
      z_angle_{z_angle},
      scale_{scale},
      begin_{begin},
      size_{size} {
}

Matrix SceneObject::GetObjectMatrix() const {
    Matrix object_matrix{1};
    object_matrix = glm::translate(object_matrix, position_);
    object_matrix = glm::rotate(object_matrix, glm::radians(x_angle_), Vector{1, 0, 0});
    object_matrix = glm::rotate(object_matrix, glm::radians(y_angle_), Vector{0, 1, 0});
    object_matrix = glm::rotate(object_matrix, glm::radians(z_angle_), Vector{0, 0, 1});
    object_matrix = glm::scale(object_matrix, Vector{scale_, scale_, scale_});
    return object_matrix;
}

Point& SceneObject::AccessPosition() {
    return position_;
}

const Point& SceneObject::AccessPosition() const {
    return position_;
}

float& SceneObject::AccessXAngle() {
    return x_angle_;
}

const float& SceneObject::AccessXAngle() const {
    return x_angle_;
}

float& SceneObject::AccessYAngle() {
    return y_angle_;
}

const float& SceneObject::AccessYAngle() const {
    return y_angle_;
}

float& SceneObject::AccessZAngle() {
    return z_angle_;
}

const float& SceneObject::AccessZAngle() const {
    return z_angle_;
}

float& SceneObject::AccessScale() {
    return scale_;
}

const float& SceneObject::AccessScale() const {
    return scale_;
}

size_t SceneObject::Begin() const {
    return begin_;
}

size_t SceneObject::Size() const {
    return size_;
}

}  // namespace renderer
