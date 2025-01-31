/**
 * @file
 * @brief Рендерер камеры в изображение
 */

#pragma once

#include "image.hpp"
#include "scene.hpp"

namespace renderer {

/**
 * @brief Позволяет рендерит изображение с заданной камеры
 */
class Renderer {
public:
    /**
     * @brief Создание рендерера
     */
    Renderer() = default;

    /**
     * @brief Рендеринг камеры в изображение
     *
     * Рендерит переданную сцену через камеру с переданным ID. Требуется, чтобы камера принадлежала
     * сцене. Параметры выходного изображения берутся из переданного изображения. Можно указать
     * горизонтальное поле зрения в градусах в диапазоне от 0.0 до 360.0 (не включая крайние
     * значения), по умолчанию 90.0. Также можно указать фокусное расстояни камеры от 0.1 до 10.0,
     * по умолчанию 1.0
     *
     * @param[in] scene Сцена
     * @param[in] camera_id ID камеры в сцене
     * @param[in] image Изображение
     * @param[in] fov_x Горизонтальное поле зрения
     *
     * @return Срендеренное изображение
     */
    Image&& Render(const Scene& scene, const Scene::CameraId camera_id, Image&& image,
                   const float fov_x = 90.0, const float focal_length = 1.0);

private:
    /**
     * @brief Инициализация переменных рендеринга
     *
     * Инициализирует внутренние переменные
     *
     * @param[in] width Ширина выходного изображения, должна быть больше 0
     * @param[in] height Высота выходного изображения, должна быть больше 0
     * @param[in] focal_length Расстояние от камеры до экрана, должно быть больше 0.0
     * @param[in] fov_x Горизонтальное поле зрения в градусах, должно быть больше 0.0 и меньше 360.0
     */
    void Init(const size_t width, const size_t height, const float focal_length, const float fov_x);

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

    struct Parameters {
        size_t width{0};
        size_t height{0};
        float fov_x{0};
        float focal_length{0};
        float x_scale{0};
        float y_scale{0};
        Matrix camera_to_clip;
    };
    Parameters parameters_;
    std::vector<float> z_buffer_;
};

};  // namespace renderer
