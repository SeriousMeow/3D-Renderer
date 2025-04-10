/**
 * @file
 * @brief Источники света
 */
#pragma once

#include <variant>

#include "renderer/color.hpp"
#include "renderer/types.hpp"

namespace renderer {

/**
 * @brief Фоновое освещение
 */
struct AmbientLight {
    /**
     * Интенсивность
     */
    float strength = 0.1;
    /**
     * Цвет
     */
    Color color = colors::kWhite;
};

/**
 * @brief Направленный источник света
 */
struct DirectionalLight {
    /**
     * Интенсивность
     */
    float strength = 1.0;
    /**
     * Цвет
     */
    Color color = colors::kWhite;
    /**
     * Направление
     */
    Vector direction = {0, 0, -1};
};

/**
 * @brief Точечный источник света
 */
struct PointLight {
    /**
     * Интенсивность
     */
    float strength = 1.0;
    /**
     * Цвет
     */
    Color color = colors::kWhite;
    /**
     * Постоянный коэффициент зависимости интенсивности от расстояния
     */
    float constant = 1;
    /**
     * Линейный коэффициент зависимости интенсивности от расстояния
     */
    float linear = 0.35;
    /**
     * Квадратичный коэффициент зависимости интенсивности от расстояния
     */
    float quadratic = 0.44;
    /**
     * Положение
     */
    Point position = {0, 0, 0};
};

/**
 * @brief Точечный направленный источник света
 */
struct SpotLight {
    /**
     * Интенсивность
     */
    float strength = 1.0;
    /**
     * Цвет
     */
    Color color = colors::kWhite;
    /**
     * Постоянный коэффициент зависимости интенсивности от расстояния
     */
    float constant = 1;
    /**
     * Линейный коэффициент зависимости интенсивности от расстояния
     */
    float linear = 0.35;
    /**
     * Квадратичный коэффициент зависимости интенсивности от расстояния
     */
    float quadratic = 0.44;
    /**
     * Положение
     */
    Point position = {0, 0, 0};
    /**
     * Экспонента
     */
    int32_t exponent = 1;
    /**
     * Направление
     */
    Vector direction = {0, 0, -1};
};

/**
 * @brief Общий тип источника света
 */
using LightSource = std::variant<AmbientLight, DirectionalLight, PointLight, SpotLight>;

}  // namespace renderer
