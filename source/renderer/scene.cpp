#include "renderer/scene.hpp"

renderer::Scene::ObjectId renderer::Scene::Push(const Object& object, const Matrix& matrix) {
    ObjectId id = objects_.size();
    objects_.push_back({.id = id, .object = object, .object_to_scene_matrix = matrix});
    return id;
}

renderer::Scene::ObjectsIterator renderer::Scene::ObjectsBegin() {
    return objects_.begin();
}

renderer::Scene::ObjectsIterator renderer::Scene::ObjectsEnd() {
    return objects_.end();
}

renderer::Matrix renderer::Scene::GetObjectMatrix(const ObjectId id) {
    {
        assert(IsObjectExists(id) and "GetObjectMatrix: объекта с переданным ID не существует");
    }
    return objects_[id].object_to_scene_matrix;
}

void renderer::Scene::SetObjectMatrix(const ObjectId id, const Matrix& new_matrix) {
    {
        assert(IsObjectExists(id) and "SetObjectMatrix: объекта с переданным ID не существует");
    }
    objects_[id].object_to_scene_matrix = new_matrix;
}

bool renderer::Scene::IsObjectExists(const ObjectId id) {
    return (0 <= id and id <= objects_.size());
}
