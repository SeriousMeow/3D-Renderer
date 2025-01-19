/**
 * @file
 * @brief 3D камера
 */

#pragma once

#include "scene.hpp"

namespace renderer {

/**
 * @brief Позволяет получать информацию об объектах из определенной точки в сцене
 */
class Camera {
public:
    /**
     * @brief Указатель на объект с матрицей перехода из пространства объекта в пространство камеры
     */
    struct CameraObject {
        Object* object_ptr;
        TransformMatrix object_to_camera_matrix;
    };
    class Iterator {
    public:
        using ValueType = CameraObject;
        using Pointer = Scene::ObjectsIterator;
        using DifferenceType = std::ptrdiff_t;

        Iterator() = delete;
        Iterator(Pointer ptr, TransformMatrix transform);
        ValueType operator*();
        Iterator& operator++();
        Iterator operator++(int);
        Iterator& operator--();
        Iterator operator--(int);
        Iterator& operator+=(DifferenceType n);
        Iterator& operator-=(DifferenceType n);
        Iterator operator+(DifferenceType n);
        Iterator operator-(DifferenceType n);
        bool operator==(const Iterator& other);
        bool operator!=(const Iterator& other);

    private:
        Pointer object_;
        TransformMatrix tranform_;
    };

    using ObjectsIterator = Iterator;

    /**
     * @brief Создание камеры
     *
     * Создает камеру, наблюдающую за переданной сценой с использованием переданной матрицы перехода
     *
     * @param[in] scene Сцена
     * @param[in] scene_to_camera Матрица перехода камеры
     */
    Camera(Scene* scene, const TransformMatrix& scene_to_camera);

    /**
     * @brief Установка новой сцены
     *
     * Устанавливает новую сцену, за которой наблюдает камера
     *
     * @param[in] new_scene Новая сцена
     * @param[in] scene_to_camera Матрица перехода камеры в новой сцене
     */
    void SetNewScene(Scene* new_scene, const TransformMatrix& scene_to_camera);

    /**
     * @brief Получение матрицы перехода камеры
     *
     * Возвращает матрицу перехода камеры
     *
     * return Матрица перехода камеры
     */
    TransformMatrix GetCameraMatrix();

    /**
     * @brief Установка новой матрицы перехода камеры
     *
     * Устанавливает новую матрицу перехода камеры
     *
     * @param[in] new_matrix Новая матрица
     */
    void SetCameraMatrix(const TransformMatrix& new_matrix);

    ObjectsIterator Begin();
    ObjectsIterator End();

private:
    Scene* scene_;
    TransformMatrix scene_to_camera_;
};

};  // namespace renderer
