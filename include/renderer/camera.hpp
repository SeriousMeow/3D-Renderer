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
    struct CameraObject {
        Object* object_ptr;
        TransformMatrix object_to_camera_matrix;
    };
    class Iterator {
    public:
        using ValueType = CameraObject;
        using Pointer = Scene::Iterator;
        using DifferenceType = std::ptrdiff_t;

        Iterator() = delete;
        Iterator(Pointer ptr, TransformMatrix tranform);
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

    using Iterator = Iterator;

    Camera(Scene* scene_ptr, TransformMatrix scene_to_camera_matrix);

    Iterator Begin();
    Iterator End();

private:
    Scene* scene_;
    TransformMatrix scene_to_camera_matrix_;
};

};  // namespace renderer
