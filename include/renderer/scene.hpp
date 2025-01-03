/**
 * @file
 * @brief 3D сцена
 */

#pragma once

#include "renderer/object.hpp"
#include "renderer/transform.hpp"

namespace renderer {

class Scene {
public:
    struct SceneObject {
        Object object;
        TransformMatrix scene_transform_matrix;
    };

    using Iterator = std::vector<SceneObject>::iterator;
    using ConstIterator = std::vector<SceneObject>::const_iterator;

    Iterator Begin();
    Iterator End();

    ConstIterator Begin() const;
    ConstIterator End() const;

private:
    std::vector<SceneObject> objects_;
};

};  // namespace renderer
