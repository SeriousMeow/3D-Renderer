#include "renderer/scene.hpp"

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
