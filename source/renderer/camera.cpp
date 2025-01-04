#include "renderer/camera.hpp"

using CameraIterator = renderer::Camera::Iterator;

CameraIterator::Iterator(CameraIterator::Pointer ptr, TransformMatrix transform)
    : object_{ptr}, tranform_{transform} {
}

CameraIterator::ValueType CameraIterator::operator*() {
    return renderer::Camera::CameraObject{
        .object_ptr = &(object_->object),
        .object_to_camera_matrix = object_->object_to_scene_matrix * tranform_};
}

CameraIterator& CameraIterator::operator++() {
    ++object_;
    return *this;
}

CameraIterator CameraIterator::operator++(int) {
    CameraIterator old{object_, tranform_};
    ++object_;
    return old;
}

CameraIterator& CameraIterator::operator--() {
    --object_;
    return *this;
}

CameraIterator CameraIterator::operator--(int) {
    CameraIterator old(object_, tranform_);
    --object_;
    return old;
}

CameraIterator& CameraIterator::operator+=(CameraIterator::DifferenceType n) {
    object_ += n;
    return *this;
}

CameraIterator& CameraIterator::operator-=(CameraIterator::DifferenceType n) {
    object_ -= n;
    return *this;
}

CameraIterator CameraIterator::operator+(CameraIterator::DifferenceType n) {
    return CameraIterator{object_ + n, tranform_};
}

CameraIterator CameraIterator::operator-(CameraIterator::DifferenceType n) {
    return CameraIterator{object_ - n, tranform_};
}

bool CameraIterator::operator==(const CameraIterator& other) {
    return (object_ == other.object_) and (tranform_ == other.tranform_);
}

bool CameraIterator::operator!=(const CameraIterator& other) {
    return not(*this == other);
}

CameraIterator renderer::Camera::Begin() {
    return CameraIterator(scene_->Begin(), scene_to_camera_matrix_);
}

CameraIterator renderer::Camera::End() {
    return CameraIterator(scene_->End(), scene_to_camera_matrix_);
}

renderer::Camera::Camera(renderer::Scene* scene, renderer::TransformMatrix scene_to_camera_matrix)
    : scene_{scene}, scene_to_camera_matrix_{scene_to_camera_matrix} {
}
