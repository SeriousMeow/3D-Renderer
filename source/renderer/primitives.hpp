/**
 * @file
 * @brief Графические примитивы
 */

#pragma once

#include "renderer/resources_types.hpp"
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
    Vector normal = {0, 0, 0};

    /**
     * @brief UV координаты вершины
     */
    Point2 uv_coordinates = {0, 0};
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

    /**
     * @brief Материал грани
     */
    MaterialId material = 0;
};
};  // namespace renderer
