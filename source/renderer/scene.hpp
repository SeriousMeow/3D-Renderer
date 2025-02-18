/**
 * @file
 * @brief 3D сцена
 */

#pragma once

#include "renderer/object.hpp"

namespace renderer {

/**
 * @brief Контейнер для объектов
 *
 * Хранит объекты и матрицы, которые описывают положение объектов в сцене
 */
class Scene {
public:
    /**
     * @brief ID объекта
     *
     * Используется для указания на объект в сцене
     */
    using ObjectId = size_t;

    /**
     * @brief ID камера
     *
     * Используется для указания на камеру в сцене
     */
    using CameraId = size_t;

    /**
     * @brief Объект в сцене
     */
    struct SceneObject {
        /**
         * @brief ID объекта
         */
        ObjectId id;

        /**
         * @brief Объект
         */
        Object object;

        /**
         * @brief Матрица объекта
         */
        Matrix object_to_scene;
    };

    /**
     * @brief Итератор объектов в сцене
     */
    using ObjectsIterator = std::vector<SceneObject>::const_iterator;

    /**
     * @brief Создание пустой сцены
     */
    Scene() = default;

    /**
     * @brief Добавление объекта в сцену
     *
     * Копирует переданный объект в контейнер. Переданная матрица является матрицей перехода из
     * координат объекта в координаты сцены
     *
     * @param[in] object Объект
     * @param[in] matrix Матрица
     *
     * @return ID добавленного объекта
     */
    ObjectId PushObject(const Object& object, const Matrix& matrix);

    /**
     * @brief Добавление камеры в сцену
     *
     * Добавляет камеру в сцену. Переданная матрица является матрицей перехода из координат сцены в
     * координаты камеры
     *
     * @param[in] matrix Матрица
     *
     * @return ID добавленной камеры
     */
    CameraId PushCamera(const Matrix& matrix);

    /**
     * @brief Получение матрицы перехода объекта
     *
     * Возвращает матрицу перехода объекта с ID, равным id. Требуется, чтобы объект с таким id
     * существовал
     *
     * @param[in] id ID объекта
     *
     * @return Матрица перехода объекта
     */
    Matrix GetObjectMatrix(const ObjectId id) const;

    /**
     * @brief Получение матрицы перехода камеры
     *
     * Возвращает матрицу перехода камеры с ID, равным id. Требуется, чтобы камера с таким id
     * существовала
     *
     * @param[in] id ID камеры
     *
     * @return Матрица перехода камеры
     */
    Matrix GetCameraMatrix(const CameraId id) const;

    /**
     * @brief Установка новой матрицы перехода объекта
     *
     * Устанавливает новую матрицу перехода объекта с ID, равным id. Требуется, чтобы объект с таким
     * id существовал
     *
     * @param[in] id ID объекта
     * @param[in] new_matrix Новая матрица перехода
     */
    void SetObjectMatrix(const ObjectId id, const Matrix& new_matrix);

    /**
     * @brief Установка новой матрицы перехода камеры
     *
     * Устанавливает новую матрицу перехода камеры с ID, равным id. Требуется, чтобы камера с таким
     * id существовала
     *
     * @param[in] id ID объекта
     * @param[in] new_matrix Новая матрица перехода
     */
    void SetCameraMatrix(const CameraId id, const Matrix& new_matrix);

    /**
     * @brief Проверка существования объекта
     *
     * Проверяет, существует ли в сцене объект с заданным ID
     *
     * @param[in] id ID объекта
     *
     * @return Существует ли объект
     */
    bool HasObject(const ObjectId id) const;

    /**
     * @brief Проверка существования камеры
     *
     * Проверяет, существует ли в сцене камера с заданным ID
     *
     * @param[in] id ID камера
     *
     * @return Существует ли камера
     */
    bool HasCamera(const CameraId id) const;

    /**
     * @brief Итератор объектов
     *
     * @return Итератор начала объектов в сцене
     */
    ObjectsIterator ObjectsBegin() const;

    /**
     * @brief Итератор объектов
     *
     * @return Итератор конца объектов в сцене
     */
    ObjectsIterator ObjectsEnd() const;

private:
    std::vector<SceneObject> objects_;
    std::vector<Matrix> cameras_;
};

};  // namespace renderer
