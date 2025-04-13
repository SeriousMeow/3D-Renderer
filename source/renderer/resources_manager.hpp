/**
 * @file
 * @brief Менеджер ресурсов
 */
#pragma once

#include <string>
#include <vector>

#include "renderer/image.hpp"
#include "renderer/resources_types.hpp"
#include "renderer/types.hpp"

namespace renderer {

/**
 * @brief Менеджер ресурсов
 *
 * Singleton класс, загружающий и хранящий материалы и текстуры. По индексам 0 содержатся материал и
 * текстура по-умолчанию
 */
class ResourcesManager {
public:
    /**
     * @brief Получение ResourcesManager
     *
     * Возвращает ссылку на ResourcesManager. Объект создается при первом запросе
     *
     * @return Ссылка на ResourcesManager
     */
    static ResourcesManager& Get();

    /**
     * @brief Добавление материала
     *
     * Сохраняет переданный материал в хранилище. Возращает ID добавленного материала
     *
     * @param[in] material Материал
     *
     * @return ID добавленного материала
     */
    MaterialId PushMaterial(const Material& material);

    /**
     * @brief Добавление текстуры
     *
     * Загружает текстуру из файла по переданному пути. Возвращает ID добавленой текстуры. Если файл
     * уже был загружен раньше, возвращает его ID и не производит повторную загрузку. В случае
     * ошибки возвращает 0 - ID текстуры по-умолчанию
     *
     * @return ID добавленной текстуры
     */
    TextureId PushTexture(const std::string& path);

    /**
     * @brief Получение доступа к материалу
     *
     * Возвращает ссылку на материал с переданным id. Требуется, чтобы материал был в хранилище
     *
     * @param[in] id ID материала
     *
     * @return Ссылка на материал
     */
    Material& AccessMaterial(const MaterialId id);

    /**
     * @brief Получение доступа к материалу
     *
     * Возвращает константную ссылку на материал с переданным id. Требуется, чтобы материал был в
     * хранилище
     *
     * @param[in] id ID материала
     *
     * @return Константная ссылка на материал
     */
    const Material& AccessMaterial(const MaterialId id) const;

    /**
     * @brief Получение цвета пикселя по UV координатам
     *
     * Возвращает цвет пикселя в текстуре с идентификатором id на переданных UV координатах. Если
     * пиксель не принадлежит текстуре, возвращается цвет, который соответсвовал бы пикселю при
     * замощении плоскости текстурой
     */
    Color GetPixelByUV(const TextureId id, const Point2& uv_coordinates) const;

    /**
     * @brief Проверка наличия материала
     *
     * Проверяет, содержится ли материал с переданным id в хранилище
     *
     * @param[in] id ID материала
     *
     * @return Содержится ли материал
     */
    bool HasMaterial(const MaterialId id) const;

    /**
     * @brief Проверка наличия текстуры
     *
     * Проверяет, содержится ли текстура с переданным id в хранилище
     *
     * @param[in] id ID текстуры
     *
     * @return Содержится ли текстура
     */
    bool HasTexture(const TextureId id) const;

private:
    struct Texture {
        std::string path;
        Image image;
    };

    /**
     * @brief Создание ResourcesManager
     */
    ResourcesManager();
    std::vector<Material> materials_;
    std::vector<Texture> textures_;
};

}  // namespace renderer
