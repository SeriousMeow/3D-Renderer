/**
 * @file
 * @brief 3D сцена
 */

#pragma once

#include "renderer/object.hpp"
#include "renderer/transform.hpp"

namespace renderer {

/*
 * @brief Хранит объекты и их положение в пространстве
 */
class Scene {
public:
    /*
     * @brief Объект с матрицей трансформации в сцене
     */
    struct SceneObject {
        Object object;
        TransformMatrix object_to_scene_matrix;
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
