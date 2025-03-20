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
    using FacetType = Triangle;
    using Iterator = std::vector<FacetType>::iterator;
    using ConstIterator = std::vector<FacetType>::const_iterator;

    /**
     * @brief Конструктор из треугольников
     *
     * @param[in] triangles треугольники
     */
    explicit Object(const std::vector<FacetType>& triangles);

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
    std::vector<FacetType> triangles_;
};

};  // namespace renderer
