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
     * сцене. Параметры выходного изображения берутся из переданного изображения
     *
     * @param[in] scene Сцена
     * @param[in] camera_id ID камеры в сцене
     * @param[in] image Изображение
     *
     * @return Срендеренное изображение
     *
     * @note Если ширина или высота изображения равны 0, то оно возвращается без изменений
     */
    Image Render(const Scene& scene, const Scene::CameraId camera_id, Image&& image);

private:
    /**
     * @brief Подготовка внутренних параметров
     *
     * Подготавливает внутреннее состояние для отрисовки нового кадра
     *
     * @param[in] width Ширина выходного изображения, должна быть больше 0
     * @param[in] height Высота выходного изображения, должна быть больше 0
     * @param[in] focal_length Расстояние от камеры до экрана
     * @param[in] fov_x Горизонтальное поле зрения в градусах
     */
    void UpdateInternalState(const size_t width, const size_t height, const float focal_length,
                             const float fov_x);

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
        float x_scale{0};
        float y_scale{0};
        Matrix camera_to_clip;
    };
    Parameters parameters_;
    std::vector<float> z_buffer_;
};

};  // namespace renderer
