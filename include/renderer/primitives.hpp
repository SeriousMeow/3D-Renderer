/**
 * @file
 * @brief Графические примитивы
 */

#pragma once

#include <glm/vec4.hpp>

namespace renderer {

/**
 * @brief Точка в пространстве
 */
using Point = glm::vec4;

/**
 * @brief Вектор в пространстве
 */
using Vector = glm::vec4;

/**
 * @brief Треугольник с векторами нормали в вершинах
 */
struct Triangle {
    Point vertices[3];
    Vector normal_vectors[3];
};
};  // namespace renderer
