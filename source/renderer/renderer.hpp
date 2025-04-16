/**
 * @file
 * @brief Рендерер камеры в изображение
 */

#pragma once

#include <cstdint>

#include "image.hpp"
#include "scene.hpp"

namespace renderer {

/**
 * @brief Позволяет рендерит изображение с заданной камеры
 */
class Renderer {
public:
    /**
     * @brief Тип для флагов рендеринга
     */
    using RenderFlags = uint32_t;

    enum RenderOptions : RenderFlags {
        /**
         * Отрисовка ребер
         */
        DRAW_EDGES = 0b1,
        /**
         * Отрисовка граней (заливка)
         */
        DRAW_FACETS = 0b10,
        /**
         * Отключение пропуска граней, обращенных от камеры
         */
        DISABLE_BACKFACE_CULLING = 0b100,
        /**
         * Рассчет освещения. Неактивно, если неактивно DRAW_FACETS
         */
        ENABLE_LIGHT = 0b1000
    };

    /**
     * @brief Создание рендерера
     */
    Renderer() = default;

    /**
     * @brief Рендеринг камеры в изображение
     *
     * Рендерит переданную сцену через камеру с переданным ID. Требуется, чтобы камера принадлежала
     * сцене. Параметры выходного изображения берутся из переданного изображения. Можно передавать
     * флаги для отрисоки, полный список которых с описанием доступен в Renderer::RenderOptions
     *
     * @param[in] scene Сцена
     * @param[in] camera_id ID камеры в сцене
     * @param[in] image Изображение
     * @param[in] flags Флаги отрисовки
     *
     * @return Срендеренное изображение
     *
     * @note Если ширина или высота изображения равны 0, то оно возвращается без изменений
     */
    Image Render(const Scene& scene, const Scene::CameraId camera_id, Image&& image,
                 const RenderFlags flags = DRAW_FACETS);

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

    /**
     * @brief Рисование треугольника
     *
     * Рисует переданный треугольник с учетом буффера глубины. Треугольник передается в camera space
     *
     * @param[out] image Изображение
     * @param[in] triangle Треугольник
     */
    void DrawTriangle(Image& image, const Triangle& triangle);

    /**
     * @brief Растеризация треугольника
     *
     * Растеризует переданный треугольник в прямоугольнике от точки (x0, y0) до (x1, y1).
     * Треугольник передается в camera space
     *
     * @param[out] image Изображение
     * @param[in] triangle Треугольник
     * @param[in] x0 x0
     * @param[in] y0 y0
     * @param[in] x1 x1
     * @param[in] y1 y1
     */
    void TriangleRasterizationTask(Image& image, const Triangle& triangle, const int32_t x0,
                                   const int32_t y0, const int32_t x1, const int32_t y1);

    /**
     * @brief Обрезка треугольника относительно пирамиды зрения
     *
     * Обрезает переданный треугольник относительно пирамиды зрения. По указателю result должен быть
     * массив на хотя бы 63 значения. После окончания работы по указателю start прописывается индекс
     * в переданном массиве, начиная с которого идет результат обрезки, количество треугольников
     * возвращается функцией. До индекса start находятся произвольные значения, после последнего
     * треугольника результата данные не изменяются. Гарантируется, что функция модифицирует не
     * больше 63 первых значений
     *
     * @param[in] triangle Треугольник для обрезки
     * @param[out] result Массив с результатом работы
     * @param[out] start Индекс начала результата в массиве
     *
     * @return Количество треугольников в результате
     */
    size_t ClipTriangle(const Triangle& triangle, Triangle* result, size_t* start);

    /**
     * Общие данные для процесса рендеринга
     */
    struct Parameters {
        size_t width{0};
        float x_scale{0};
        float y_scale{0};
        Matrix camera_to_clip;
        Vector4 frustum_planes[5];
        Scene::LightConstIterator light_begin;
        Scene::LightConstIterator light_end;
        Matrix scene_to_camera;
    };

    /**
     * Общие данные для процесса отрисовки треугольника
     */
    struct DrawParameters {
        Vector inv_w;         // 1/A.w, 1/B.w, 1/C.w
        Point vertices[3];    // вершины в clip space
        MaterialId material;  // материал грани
    };

    Parameters parameters_;
    DrawParameters draw_parameters_;
    RenderFlags flags_;
    std::vector<float> z_buffer_;
};

};  // namespace renderer
