/**
 * @file
 * @brief 3D объект
 */

#pragma once

#include <vector>

#include "renderer/primitives.hpp"

namespace renderer {

/**
 * @brief 3D объект - контейнер для графических примитивов
 */
class Object {
public:
    using Iterator = std::vector<Triangle>::iterator;
    using ConstIterator = std::vector<Triangle>::const_iterator;

    Iterator Begin();
    Iterator End();

    ConstIterator Begin() const;
    ConstIterator End() const;

private:
    std::vector<Triangle> triangles_;
};

};  // namespace renderer
