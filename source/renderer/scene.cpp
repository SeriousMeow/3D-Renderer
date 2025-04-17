#include "renderer/scene.hpp"

namespace renderer {

Scene::ObjectId Scene::PushObject(const Object& object) {
    ObjectId id = objects_.size();
    size_t new_object_start = facets_storage_.size();
    size_t new_object_size = std::distance(object.Begin(), object.End());
    facets_storage_.reserve(facets_storage_.size() + new_object_size);
    for (auto it = object.Begin(); it != object.End(); ++it) {
        facets_storage_.push_back(*it);
    }
    objects_.emplace_back(new_object_start, new_object_size);
    return id;
}

Scene::CameraId Scene::PushCamera(const Camera& camera) {
    CameraId id = cameras_.size();
    cameras_.push_back(camera);
    return id;
}

Scene::LightId Scene::PushLight(const LightSource& light_source) {
    LightId id = light_sources_.size();
    light_sources_.push_back(light_source);
    return id;
}

Scene::ObjectsIterator Scene::ObjectsBegin() {
    return objects_.begin();
}

Scene::ObjectsConstIterator Scene::ObjectsBegin() const {
    return objects_.begin();
}

Scene::ObjectsIterator Scene::ObjectsEnd() {
    return objects_.end();
}

Scene::ObjectsConstIterator Scene::ObjectsEnd() const {
    return objects_.end();
}

Scene::LightIterator Scene::LightBegin() {
    return light_sources_.begin();
}

Scene::LightConstIterator Scene::LightBegin() const {
    return light_sources_.begin();
}

Scene::LightIterator Scene::LightEnd() {
    return light_sources_.end();
}

Scene::LightConstIterator Scene::LightEnd() const {
    return light_sources_.end();
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

SceneObject& Scene::AccessObject(const ObjectId id) {
    {
        assert(HasObject(id) and "AccessObject: объекта с переданным ID не существует");
    }
    return objects_[id];
}

const SceneObject& Scene::AccessObject(const ObjectId id) const {
    {
        assert(HasObject(id) and "AccessObject: объекта с переданным ID не существует");
    }
    return objects_[id];
}

LightSource& Scene::AccessLight(const LightId id) {
    {
        assert(HasLight(id) and "AccessLight: источника света с переданным ID не существует");
    }
    return light_sources_[id];
}

const LightSource& Scene::AccessLight(const LightId id) const {
    {
        assert(HasLight(id) and "AccessLight: источника света с переданным ID не существует");
    }
    return light_sources_[id];
}

bool Scene::HasObject(const ObjectId id) const {
    return (0 <= id and id < objects_.size());
}

bool Scene::HasCamera(const CameraId id) const {
    return (0 <= id and id < cameras_.size());
}

bool Scene::HasLight(const LightId id) const {
    return (0 <= id and id < light_sources_.size());
}

Object::FacetType* Scene::AccessFacetsStorage() {
    return facets_storage_.data();
}

const Object::FacetType* Scene::AccessFacetsStorage() const {
    return facets_storage_.data();
}

}  // namespace renderer
