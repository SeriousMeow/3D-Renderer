/**
 * @file
 * @brief Камера
 */

#pragma once

#include "renderer/transform.hpp"
#include "renderer/types.hpp"

namespace renderer {

/**
 * @brief Камера в 3D пространстве
 */
class Camera {
public:
    /**
     * @brief Создание камеры
     *
     * Горизонтальное поле зрения задается в градусах в диапазоне от 0.0 до 360.0 (не включая
     * крайние значения). По умолчанию 90.0
     *
     * Матрица задает изначальное расположение камеры в сцене,
     * по умолчанию камера расположена в точке (0, 0, 0), верх изображения соответсвует
     * положительному направлению оси y, правая сторона изображения соответствует положительному
     * направлению оси x, камера смотрит вдоль отрицательного направления оси z
     *
     * Фокусное расстояние задается в диапазоне от 0.1 до 10.0. По умолчанию 1.0
     *
     * @param[in] fov_x Горизонтальное поле зрения
     * @param[in] scene_to_camera Матрица расположения камеры в сцене
     * @param[in] focal_length Фокусное расстояние
     */
    explicit Camera(const Matrix& scene_to_camera = transforms::kNoTransforms,
                    const float fov_x = 90, const float focal_length = 1.0);

    /**
     * @brief Получение доступа к матрице
     *
     * Возвращает ссылку на матрицу расположения камеры в сцене
     *
     * @return Ссылка на матрицу
     */
    Matrix& AccessMatrix();

    /**
     * @brief Получение доступа к матрице
     *
     * Возвращает константуную ссылку на матрицу расположения камеры в сцене
     *
     * @return Константная ссылка на матрицу
     */
    const Matrix& AccessMatrix() const;

    /**
     * @brief Получение текущего горизонтального угла обзора
     *
     * Возвращает текущий горизонтальный угол обзора в градусах
     *
     * @return Угол обзора
     */
    float GetFovX() const;

    /**
     * @brief Установка нового горизонтального угла обзора
     *
     * Устанавливает новый горизонтальный угол обзора в градусах. Должен быть в диапазоне от 0.0 до
     * 360.0 не включая границы
     *
     * @param[in] new_fov_x Новый угол обзора
     */
    void SetFovX(const float new_fov_x);

    /**
     * @brief Получение текущего фокусного расстояния
     *
     * Возвращает текущее фокусное расстояние камеры
     *
     * @return Фокусное расстояние
     */
    float GetFocalLenght() const;

    /**
     * @brief Установка нового фокусного расстояния
     *
     * Устанавливает новое фокусное расстояние. Должно быть в диапазоне от 0.1 до 10.0 включая
     * границы
     */
    void SetFocalLength(const float new_focal_lenght);

private:
    Matrix scene_to_camera_;
    float fov_x_;
    float focal_length_;
};

}  // namespace renderer
