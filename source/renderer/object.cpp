#include <renderer/object.hpp>

namespace renderer {

Object::Object(const std::vector<Triangle>& triangles) : triangles_{triangles} {
}

Object::Iterator Object::Begin() {
    return triangles_.begin();
}

Object::Iterator Object::End() {
    return triangles_.end();
}

Object::ConstIterator Object::Begin() const {
    return triangles_.begin();
}

Object::ConstIterator Object::End() const {
    return triangles_.end();
}
}  // namespace renderer
