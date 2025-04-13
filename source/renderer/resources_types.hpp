/**
 * @file
 * @brief Типы ресурсов
 */
#pragma once

#include <cstdint>

#include "renderer/color.hpp"

namespace renderer {
/**
 * @brief Тип ID материала
 */
using MaterialId = int64_t;

/**
 * @brief Тип ID текстуры
 */
using TextureId = int64_t;

/**
 * @brief Материал
 */
struct Material {

    /** Базовый цвет фонового света */
    Color ambient = colors::kWhite;

    /** Базовый цвет рассеяного света */
    Color diffuse = colors::kWhite;

    /** Базовый цвет отраженного света */
    Color specular = colors::kWhite;

    /** Блеск материала */
    float shininess = 2;

    /** Нужно ли отрисовывать с обеих сторон */
    bool two_sided = false;

    /** Текстура */
    TextureId texture = 0;
};

}  // namespace renderer
