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
     * @brief Объект с матрицей трансформации в сцене
     */
    struct SceneObject {
        Object object;
        TransformMatrix object_to_scene_matrix;
    };

    using Iterator = std::vector<SceneObject>::iterator;
    using ConstIterator = std::vector<SceneObject>::const_iterator;

    /**
     * @brief Создание пустой сцены
     */
    Scene() = default;

    /**
     * @brief Создание сцены из объектов и матриц
     *
     * @param[in] objects Объекты
     */
    explicit Scene(const std::vector<SceneObject>& objects);

    /**
     * @brief Добавление объекта в сцену
     *
     * Копирует переданный объект в контейнер. Переданная матрица является матрицей перехода из
     * координат объекта в координаты сцены
     *
     * @param[in] object Объект
     * @param[in] matrix Матрица
     */
    void Push(const Object& object, const TransformMatrix& matrix);

    /**
     * @brief Итератор начала контейнера
     *
     * return Итератор начала контейнера
     */
    Iterator Begin();

    /**
     * @brief Итератор конца контейнера
     *
     * return Итератор конца контейнера
     */
    Iterator End();

    /**
     * @brief Итератор начала контейнера
     *
     * return Итератор начала контейнера
     */
    ConstIterator Begin() const;

    /**
     * @brief Итератор конца контейнера
     *
     * return Итератор конца контейнера
     */
    ConstIterator End() const;

private:
    std::vector<SceneObject> objects_;
};

};  // namespace renderer
