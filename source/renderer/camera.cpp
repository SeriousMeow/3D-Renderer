#include "renderer/camera.hpp"

using CameraIterator = renderer::Camera::ObjectsIterator;

CameraIterator::Iterator(CameraIterator::Pointer ptr, Matrix transform)
    : object_{ptr}, tranform_{transform} {
}

CameraIterator::ValueType CameraIterator::operator*() {
    return renderer::Camera::CameraObject{
        .object_ptr = &(object_->object),
        .object_to_camera_matrix = tranform_ * object_->object_to_scene_matrix};
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
    return CameraIterator(scene_->ObjectsBegin(), scene_to_camera_);
}

CameraIterator renderer::Camera::End() {
    return CameraIterator(scene_->ObjectsEnd(), scene_to_camera_);
}

renderer::Camera::Camera(renderer::Scene* scene, const renderer::Matrix& scene_to_camera)
    : scene_{scene}, scene_to_camera_{scene_to_camera} {
    {
        assert(scene and "Scene: scene не должен быть nullptr");
    }
}

void renderer::Camera::SetNewScene(Scene* new_scene, const Matrix& scene_to_camera) {
    {
        assert(new_scene and "SetNewScene: new_scene не доллжен быть nullptr");
    }
    scene_ = new_scene;
    scene_to_camera_ = scene_to_camera;
}

renderer::Matrix renderer::Camera::GetCameraMatrix() {
    return scene_to_camera_;
}
