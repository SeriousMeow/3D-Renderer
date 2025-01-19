/**
 * @file
 * @brief 3D сцена
 */

#pragma once

#include "renderer/object.hpp"
#include "renderer/transform.hpp"

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
     * Используется указания на объект в сцене
     */
    using ObjectId = size_t;

    /**
     * @brief Объект с матрицей трансформации в сцене
     */
    struct SceneObject {
        ObjectId id;
        Object object;
        TransformMatrix object_to_scene_matrix;
    };

    using ObjectsIterator = std::vector<SceneObject>::iterator;

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
     * return ID добавленного объекта
     */
    ObjectId Push(const Object& object, const TransformMatrix& matrix);

    /**
     * @brief Получение матрицы перехода объекта
     *
     * Возвращает матрицу перехода объекта с ID, равным id. Требуется, чтобы объект с таким id
     * существовал
     *
     * @param[in] id ID объекта
     *
     * return Матрица перехода объекта
     */
    TransformMatrix GetObjectMatrix(const ObjectId id);

    /**
     * @brief Установка новой матрицы перехода объекта
     *
     * Устанавливает новую матрицу перехода объекта с ID, равным id. Требуется, чтобы объект с таким
     * id существовал
     *
     * @param[in] id ID объекта
     * @param[in] new_matrix Новая матрица перехода
     */
    void SetObjectMatrix(const ObjectId id, const TransformMatrix& new_matrix);

    /**
     * @brief Проверка существования объекта
     *
     * Проверяет, существует ли в сцене объект с заданным ID
     *
     * @param[in] id ID объекта
     *
     * return Существует ли объект
     */
    bool IsObjectExists(const ObjectId id);

    /**
     * @brief Итератор начала контейнера
     *
     * return Итератор начала контейнера
     */
    ObjectsIterator ObjectsBegin();

    /**
     * @brief Итератор конца контейнера
     *
     * return Итератор конца контейнера
     */
    ObjectsIterator ObjectsEnd();

private:
    std::vector<SceneObject> objects_;
};

};  // namespace renderer
