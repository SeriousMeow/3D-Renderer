/**
 * @file
 * @brief 3D сцена
 */

#pragma once

#include "renderer/camera.hpp"
#include "renderer/object.hpp"
#include "renderer/scene_object.hpp"

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
     * @brief Итератор объектов в сцене
     */
    using ObjectsIterator = std::vector<SceneObject>::iterator;
    using ObjectsConstIterator = std::vector<SceneObject>::const_iterator;

    /**
     * @brief Добавление объекта в сцену
     *
     * Копирует переданный объект в контейнер, его характеристики выставляются по-умолчанию для
     * SceneObject
     *
     * @param[in] object Объект
     *
     * @return ID добавленного объекта
     */
    ObjectId PushObject(const Object& object);

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
    CameraId PushCamera(const Camera& camera);

    /**
     * @brief Получение доступа к объекту
     *
     * Возвращает ссылку на SceneObject с информацией об объекте
     *
     * @param[in] id ID объекта
     *
     * @return Ссылка доступа к объекту
     */
    SceneObject& AccessObject(const ObjectId id);

    /**
     * @brief Получение доступа к объекту
     *
     * Возвращает константную ссылку на SceneObject с информацией об объекте
     *
     * @param[in] id ID объекта
     *
     * @return Константная ссылка доступа к объекту
     */
    const SceneObject& AccessObject(const ObjectId id) const;

    /**
     * @brief Получение доступа к камере
     *
     * Возвращает ссылку на камеру в сцене с ID, равным id. Требуется, чтобы камера с таким id
     * существовала
     *
     * @param[in] id ID камеры
     *
     * @return Ссылка на камеру
     */
    Camera& AccessCamera(const CameraId id);

    /**
     * @brief Получение доступа к камере
     *
     * Возвращает константную ссылку на камеру в сцене с ID, равным id. Требуется, чтобы камера с
     * таким id существовала
     *
     * @param[in] id ID камеры
     *
     * @return Константная ссылка на камеру
     */
    const Camera& AccessCamera(const CameraId id) const;

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
     * @brief Итератор начала объектов
     *
     * Возвращает итератор начала объектов в сцене
     *
     * @return Итератор начала объектов
     */
    ObjectsIterator ObjectsBegin();

    /**
     * @brief Итератор начала объектов
     *
     * Возвращает константный итератор начала объектов в сцене
     *
     * @return Константный итератор начала объектов
     */
    ObjectsConstIterator ObjectsBegin() const;

    /**
     * @brief Итератор конца объектов
     *
     * Возвращает итератор конца объектов в сцене
     *
     * @return Итератор конца объектов
     */
    ObjectsIterator ObjectsEnd();

    /**
     * @brief Итератор конца объектов
     *
     * Возвращает константный итератор конца объектов в сцене
     *
     * @return Константый итератор конца объектов
     */
    ObjectsConstIterator ObjectsEnd() const;

    /**
     * @brief Доступ к хранилищу граней
     *
     * Возвращает указатель на хранилище граней
     *
     * Гарантируется корректность указателя до любых,
     * изменяющих объект сцены
     * Гарантируется, что при наличии хотя бы одного объекта в сцене этот
     * указатель не nullptr
     * Гарантируется, что для любого существующего в сцене SceneObject по этому указателю, начиная с
     * индекса начала SceneObject находятся все грани объекта
     *
     * @return Указатель на хранилище граней
     */
    Object::FacetType* AccessFacetsStorage();

    /**
     * @brief Доступ к хранилищу граней
     *
     * Возвращает константный указатель на хранилище граней
     *
     * Гарантируется корректность указателя до любых,
     * изменяющих объект сцены
     * Гарантируется, что при наличии хотя бы одного объекта в сцене этот
     * указатель не nullptr
     * Гарантируется, что для любого существующего в сцене SceneObject по этому указателю, начиная с
     * индекса начала SceneObject находятся все грани объекта
     *
     * @return Константный указатель на хранилище граней
     */
    const Object::FacetType* AccessFacetsStorage() const;

private:
    std::vector<Object::FacetType> facets_storage_;
    std::vector<SceneObject> objects_;
    std::vector<Camera> cameras_;
};

};  // namespace renderer
