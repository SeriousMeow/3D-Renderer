#include "renderer/scene.hpp"

renderer::Scene::Scene(const std::vector<SceneObject>& objects) : objects_{objects} {
}

void renderer::Scene::Push(const Object& object, const TransformMatrix& matrix) {
    objects_.push_back({.object = object, .object_to_scene_matrix = matrix});
}

renderer::Scene::Iterator renderer::Scene::Begin() {
    return objects_.begin();
}

renderer::Scene::Iterator renderer::Scene::End() {
    return objects_.end();
}

renderer::Scene::ConstIterator renderer::Scene::Begin() const {
    return objects_.begin();
}

renderer::Scene::ConstIterator renderer::Scene::End() const {
    return objects_.end();
}
