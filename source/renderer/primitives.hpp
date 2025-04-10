/**
 * @file
 * @brief Графические примитивы
 */

#pragma once

#include "renderer/types.hpp"

namespace renderer {

/**
 * @brief Вершина
 *
 * Вершина в пространстве
 */
struct Vertex {
    /**
     * @brief Точка, задаваемая вершиной
     *
     * Хранит точку в декартовом пространстве, задаваемую вершиной
     */
    Point point;

    /**
     * @brief Нормаль
     */
    Vector normal;
};

/**
 * @brief Треугольная грань
 */
struct Triangle {
    /**
     * @brief Вершины грани
     *
     * Задают 3 вершины грани. Внешней стороной считается та сторона, относительно которой вершины
     * перечислены против часовой стрелки
     */
    Vertex vertices[3];
};
};  // namespace renderer
