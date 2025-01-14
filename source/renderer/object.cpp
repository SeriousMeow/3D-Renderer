#include <renderer/object.hpp>

renderer::Object::Object(const std::vector<Triangle>& triangles) : triangles_{triangles} {
}

renderer::Object::Iterator renderer::Object::Begin() {
    return triangles_.begin();
}

renderer::Object::Iterator renderer::Object::End() {
    return triangles_.end();
}

renderer::Object::ConstIterator renderer::Object::Begin() const {
    return triangles_.begin();
}

renderer::Object::ConstIterator renderer::Object::End() const {
    return triangles_.end();
}
