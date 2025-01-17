/**
 * @file
 * @brief Определения типов матриц преобразования
 */

#pragma once

#include <glm/mat4x4.hpp>

#include "primitives.hpp"

namespace renderer {

using TransformMatrix = glm::mat4;

/**
 * @namespace
 * @brief Операции с матрицами преобразований
 *
 * Содержит методы, позволяющие создавать матрицы преобразований с нужными свойствами. Также
 * содержит методы обработки матриц преобразований и предпосчитанные матрицы
 */
namespace transfroms {
/**
 * @brief Матрица, не выполняющая преобразований
 *
 * Не производит никаких преобразований. Совмещает начала и направления координатных осей без
 * изменения масштаба
 */
const TransformMatrix kNoTransforms{1.0f};

/**
 * @brief Сдвиг заданным вектором
 *
 * Выполняет сдвиг на вектор, переданного в direction
 *
 * @param[in] direction Вектор, на который производится сдвиг
 */
TransformMatrix Translate(const Vector& direction);

/**
 * @brief Растяжение вдоль координатных осей
 *
 * Выполняет растяжение вдоль координатных осей с соответвующими коэффициентами
 *
 * @param[in] x_scale_factor Коэффициент растяжения оси x
 * @param[in] y_scale_factor Коэффициент растяжения оси y
 * @param[in] z_scale_factor Коэффициент растяжения оси z
 */
TransformMatrix Scale(const float x_scale_factor, const float y_scale_factor,
                      const float z_scale_factor);

/**
 * @brief Растяженне вдоль всех координатных осей
 *
 * Выполняет растяжении вдоль всех координатных осей в scale_factor раз
 *
 * @param[in] scale_factor Коэффициент растяжения
 */
TransformMatrix ScaleUniform(const float scale_factor);

/**
 * @brief Поворот вокруг оси
 *
 * Выполняет поворот вокруг заданной оси axis на angle градусов против часовой стрелки. Угол может
 * быть отрицательным, что соотвествует повороту по часовой стрелке
 *
 * @param[in] angle Угол поворота в градусах
 * @param[in] axis Ось
 */
TransformMatrix RotateDegrees(const float angle, const Vector& axis);

/**
 * @brief Поворот вокруг оси
 *
 * Выполняет поворот вокруг заданной оси axis на angle радиан против часовой стрелки. Угол может
 * быть отрицательным, что соотвествует повороту по часовой стрелке
 *
 * @param[in] angle Угол поворота в радианах
 * @param[in] axis Ось
 */
TransformMatrix RotateRadians(const float angle, const Vector& axis);

/**
 * @brief Поворот вокруг оси z
 *
 * Выполняет поворот вокруг оси z против часовой стрелки на angle градусов. Угол может быть
 * отрицательным, что соответсвует повороту по часовой стрелке
 *
 * @param[in] angle Угол поворота
 */
TransformMatrix RotateAboutAxisZ(const float angle);

/**
 * @brief Поворот вокруг оси x
 *
 * Выполняет поворот вокруг оси x против часовой стрелки на angle градусов. Угол может быть
 * отрицательным, что соответсвует повороту по часовой стрелке
 *
 * @param[in] angle Угол поворота
 */
TransformMatrix RotateAboutAxisX(const float angle);

/**
 * @brief Поворот вокруг оси y
 *
 * Выполняет поворот вокруг оси y против часовой стрелки на angle градусов. Угол может быть
 * отрицательным, что соотвествует повороту по часовой стрелке
 *
 * @param[in] angle Угол поворота
 */
TransformMatrix RotateAboutAxisY(const float angle);

/**
 * @brief Направление камеры на точку
 *
 * Создает матрицу для камеры, расположенной в точке camera_position и смотрящей в точку center
 *
 * @param[in] camera_position Положение камеры
 * @param[in] center Точка, в которую смотрит камера
 */
TransformMatrix CameraLookAtPoint(const Point& camera_position, const Point& center);

/**
 * @brief Направление камеры в заданное направление
 *
 * Создает матрицу для камеры, расположенной в точке camera_position и смотрящей по направлению
 * direction
 *
 * @param[in] camera_position Положение камеры
 * @param[in] direction Направление взгляда
 */
TransformMatrix CameraLookAtDirection(const Point& camera_position, const Vector& direction);

};  // namespace transfroms
};  // namespace renderer
