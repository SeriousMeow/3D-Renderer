#include "renderer/scene.hpp"

namespace renderer {

Scene::ObjectId Scene::PushObject(const Object& object, const Matrix& matrix) {
    ObjectId id = objects_.size();
    objects_.push_back({.id = id, .object = object, .object_to_scene = matrix});
    return id;
}

Scene::CameraId Scene::PushCamera(const Camera& camera) {
    CameraId id = cameras_.size();
    cameras_.push_back(camera);
    return id;
}

Scene::ObjectsIterator Scene::ObjectsBegin() const {
    return objects_.begin();
}

Scene::ObjectsIterator Scene::ObjectsEnd() const {
    return objects_.end();
}

Matrix Scene::GetObjectMatrix(const ObjectId id) const {
    {
        assert(HasObject(id) and "GetObjectMatrix: объекта с переданным ID не существует");
    }
    return objects_[id].object_to_scene;
}

void Scene::SetObjectMatrix(const ObjectId id, const Matrix& new_matrix) {
    {
        assert(HasObject(id) and "SetObjectMatrix: объекта с переданным ID не существует");
    }
    objects_[id].object_to_scene = new_matrix;
}

Camera& Scene::AccessCamera(const CameraId id) {
    {
        assert(HasCamera(id) and "GetCamera: камеры с переданным ID не существует");
    }
    return cameras_[id];
}

const Camera& Scene::AccessCamera(const CameraId id) const {
    {
        assert(HasCamera(id) and "GetCamera: камеры с переданным ID не существует");
    }
    return cameras_[id];
}

bool Scene::HasObject(const ObjectId id) const {
    return (0 <= id and id < objects_.size());
}

bool Scene::HasCamera(const CameraId id) const {
    return (0 <= id and id < cameras_.size());
}
}  // namespace renderer
