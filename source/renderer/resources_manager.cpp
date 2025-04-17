#include "renderer/resources_manager.hpp"

#include "stb_image.h"

namespace renderer {

ResourcesManager& ResourcesManager::Get() {
    static ResourcesManager manager;
    return manager;
}

MaterialId ResourcesManager::PushMaterial(const Material& material) {
    MaterialId id = materials_.size();
    materials_.push_back(material);
    return id;
}

TextureId ResourcesManager::PushTexture(const std::string& path) {
    for (TextureId i = 0; i < textures_.size(); ++i) {
        if (textures_[i].path == path) {
            return i;
        }
    }
    int width, height, nr_channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nr_channels, 3);
    if (data == nullptr) {
        return 0;
    }
    Texture new_texture{
        .path{path},
        .image{Width{static_cast<size_t>(width)}, Height{static_cast<size_t>(height)}}};
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            size_t index = (x + y * width) * 3;
            uint8_t r = data[index];
            uint8_t g = data[index + 1];
            uint8_t b = data[index + 2];
            new_texture.image.AccessPixel(x, y) = {r, g, b};
        }
    }
    TextureId id = textures_.size();
    textures_.push_back(std::move(new_texture));
    stbi_image_free(data);
    return id;
}

Material& ResourcesManager::AccessMaterial(const MaterialId id) {
    {
        assert(HasMaterial(id) and "AccessMaterial: материал должен быть в хранилище");
    }
    return materials_[id];
}

const Material& ResourcesManager::AccessMaterial(const MaterialId id) const {
    {
        assert(HasMaterial(id) and "AccessMaterial: материал должен быть в хранилище");
    }
    return materials_[id];
}

Color ResourcesManager::GetPixelByUV(const TextureId id, const Point2& uv_coordinates) const {
    {
        assert(HasTexture(id) and "GetPixelByUV: текстура должна быть в хранилище");
    }
    const Texture& texture = textures_[id];
    size_t width = texture.image.GetWidth();
    size_t heigh = texture.image.GetHeight();
    int64_t x = uv_coordinates.x * width;
    int64_t y = uv_coordinates.y * heigh;

    x %= width;
    while (x < 0) {
        x += width;
    }

    y %= heigh;
    while (y < 0) {
        y += heigh;
    }

    return Image::Pixel::ToColor(texture.image.AccessPixel(x, y));
}

bool ResourcesManager::HasMaterial(const MaterialId id) const {
    return (0 <= id and id < materials_.size());
}

bool ResourcesManager::HasTexture(const TextureId id) const {
    return (0 <= id and id < textures_.size());
}

ResourcesManager::ResourcesManager() {
    materials_.emplace_back();
    Texture default_texture{.path{""}, .image{Width{1}, Height{1}}};
    default_texture.image.AccessPixel(0, 0) = {255, 255, 255};
    textures_.push_back(std::move(default_texture));
}

}  // namespace renderer
