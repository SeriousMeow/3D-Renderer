/**
 * @file
 * @brief Цвета
 */
#pragma once

#include <glm/vec3.hpp>

namespace renderer {

/**
 * @brief Цвет
 *
 * RGB цвет. Каждая компонента должна находиться в диапазоне от 0.0 до 1.0
 */
using Color = glm::vec3;

/**
 * @brief Набор встроенных цветов
 */
namespace colors {
inline constexpr Color kWhite{1, 1, 1};
inline constexpr Color kBlack{0, 0, 0};
inline constexpr Color kRed{1, 0, 0};
inline constexpr Color kGreen{0, 1, 0};
inline constexpr Color kBlue{0, 0, 1};
};  // namespace colors

}  // namespace renderer
