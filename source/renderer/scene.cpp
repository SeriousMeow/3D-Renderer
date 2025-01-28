#include "renderer/scene.hpp"

renderer::Scene::ObjectId renderer::Scene::PushObject(const Object& object, const Matrix& matrix) {
    ObjectId id = objects_.size();
    objects_.push_back({.id = id, .object = object, .object_to_scene = matrix});
    return id;
}

renderer::Scene::CameraId renderer::Scene::PushCamera(const Matrix& matrix) {
    CameraId id = cameras_.size();
    cameras_.push_back(matrix);
    return id;
}

renderer::Scene::ObjectsIterator renderer::Scene::ObjectsBegin() const {
    return objects_.begin();
}

renderer::Scene::ObjectsIterator renderer::Scene::ObjectsEnd() const {
    return objects_.end();
}

renderer::Matrix renderer::Scene::GetObjectMatrix(const ObjectId id) const {
    {
        assert(HasObject(id) and "GetObjectMatrix: объекта с переданным ID не существует");
    }
    return objects_[id].object_to_scene;
}

renderer::Matrix renderer::Scene::GetCameraMatrix(const CameraId id) const {
    {
        assert(HasCamera(id) and "GetCameraMatrix: камеры с переданным ID не существует");
    }
    return cameras_[id];
}

void renderer::Scene::SetObjectMatrix(const ObjectId id, const Matrix& new_matrix) {
    {
        assert(HasObject(id) and "SetObjectMatrix: объекта с переданным ID не существует");
    }
    objects_[id].object_to_scene = new_matrix;
}

void renderer::Scene::SetCameraMatrix(const CameraId id, const Matrix& new_matrix) {
    {
        assert(HasCamera(id) and "SetCameraMatrix: камеры с переданным ID не существует");
    }
    cameras_[id] = new_matrix;
}

bool renderer::Scene::HasObject(const ObjectId id) const {
    return (0 <= id and id < objects_.size());
}

bool renderer::Scene::HasCamera(const CameraId id) const {
    return (0 <= id and id < objects_.size());
}
