/*
 * @file
 * @brief Изображение
 */

#pragma once

#include <vector>

namespace renderer {

/*
 * @brief Изображение
 *
 * Представляет из себя линейный массив пикселей
 */
class Image {
public:
    /*
     * @brief Пиксель
     *
     * Каждая компонента цвета имеет диапазон от 0.0 до 1.0
     */
    struct Pixel {
        float r;
        float g;
        float b;
    };

    /*
     * @brief Создание изображения
     *
     * @param[in] width Ширина изображения
     * @param[in] height Высота изображения
     */
    Image(const size_t width, const size_t height);

    /*
     * @brief Получение ширины изображения
     *
     * @return Ширина изображения
     */
    size_t GetWidth() const;

    /*
     * @brief Получение высоты изображения
     *
     * @return Высота изображения
     */
    size_t GetHeight() const;

    /*
     * @brief Получение ссылки на массив изображения
     *
     * @return Ссылка на массив изображения
     */
    std::vector<Pixel>& GetImageReference();

private:
    size_t width_;
    size_t height_;
    std::vector<Pixel> image_;
};
};  // namespace renderer
