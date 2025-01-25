/**
 * @file
 * @brief 3D объект
 */

#pragma once

#include <vector>

#include "renderer/primitives.hpp"

namespace renderer {

/**
 * @brief Контейнер для графических примитивов
 */
class Object {
public:
    using Iterator = std::vector<Triangle>::iterator;
    using ConstIterator = std::vector<Triangle>::const_iterator;

    /**
     * @brief Конструктор из треугольников
     *
     * @param[in] triangles треугольники
     */
    explicit Object(const std::vector<Triangle>& triangles);

    /**
     * @brief Итератор начала контейнера
     *
     * return Итератор начала контейнера
     */
    Iterator Begin();

    /**
     * @brief Итератор конца контейнера
     *
     * return Итератор конца контейнера
     */
    Iterator End();

    /**
     * @brief Итератор начала контейнера
     *
     * return Итератор начала контейнера
     */
    ConstIterator Begin() const;

    /**
     * @brief Итератор конца контейнера
     *
     * return Итератор конца контейнера
     */
    ConstIterator End() const;

private:
    std::vector<Triangle> triangles_;
};

};  // namespace renderer
