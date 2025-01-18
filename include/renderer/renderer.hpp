/**
 * @file
 * @brief Рендерер камеры в изображение
 */

#pragma once

#include "camera.hpp"
#include "image.hpp"

namespace renderer {

/**
 * @brief Рендерит камеру в изображение
 */
class Renderer {
public:
    /**
     * @brief Создание рендерера из камеры
     *
     * @param[in] camera Камера
     */
    explicit Renderer(Camera* camera);

    /**
     * @brief Инициализация переменных рендеринга
     *
     * Инициализирует внутренние переменные, для вычисления которых нужны параметры выходного
     * изображения. Также используется для изменения параметров отрисовки
     *
     * @param[in] width Ширина выходного изображения, должна быть больше 0
     * @param[in] height Высота выходного изображения, должна быть больше 0
     * @param[in] near_plane_distance Расстояние от камеры до экрана, должно быть больше 0.0
     * @param[in] fov_x Горизонтальное поле зрения в градусах, должно быть больше 0.0 и меньше 360.0
     */
    void Init(const size_t width, const size_t height, const float near_plane_distance = 1.0,
              const float fov_x = 90.0);

    /**
     * @brief Рендеринг камеры в изображение
     *
     * Перед вызовом Render обязательно должна быть произведена инициализация с помощью функции Init
     *
     * @param[out] image Изображение, в которое будет сохранен результат
     */
    void Render(Image& image);

private:
    /**
     * @brief Рисование отрезка
     *
     * Рисует отрезок от точки start до end на image с учетом буффера глубины
     *
     * @param[out] image Изображение
     * @param[in] start Начало отрезка
     * @param[in] end Конец отрезка
     */
    void DrawLine(Image& image, const Point& start, const Point& end);
    std::vector<float> z_buffer_;
    Camera* camera_;
    size_t width_{0};
    size_t height_{0};
    float x_scale_factor_;
    float y_scale_factor_;
    TransformMatrix camera_to_clip_;
};

};  // namespace renderer
