/**
 * @file
 * @brief Изображение
 */

#pragma once

#include <cstdint>
#include <vector>

#include "renderer/color.hpp"

namespace renderer {

/**
 * @brief Ширина изображения
 */
enum Width : size_t;
/**
 * @brief Высота изображения
 */
enum Height : size_t;

/**
 * @brief Изображение
 *
 * Объект для хранения изображения, получаемого в процессе рендеринга
 */
class Image {
public:
    /**
     * @brief Пиксель
     *
     * Хранит пиксель в формате RGB24 (8 бит на цвет)
     */
    struct Pixel {
        uint8_t r;
        uint8_t g;
        uint8_t b;

        /**
         * @brief Преобразование пикселя в цвет
         *
         * Пробразует переданный пиксель в цвет
         *
         * @param[in] pixel Пиксель
         *
         * @return Цвет
         */
        static Color ToColor(const Pixel& pixel);
    };

    /**
     * @brief Создание изображения
     *
     * @param[in] width Ширина изображения
     * @param[in] height Высота изображения
     */
    Image(const Width width, const Height height);

    /**
     * @brief Получение ширины изображения
     *
     * @return Ширина изображения
     */
    size_t GetWidth() const;

    /**
     * @brief Получение высоты изображения
     *
     * @return Высота изображения
     */
    size_t GetHeight() const;

    /**
     * @brief Получение доступа к пикселю
     *
     * Возвращает ссылку на пиксель с координатами (x, y)
     *
     * @param[in] x Столбец
     * @param[in] y Столбец
     *
     * @return Ссылка на пиксель
     */
    Pixel& AccessPixel(const size_t x, const size_t y);

    /**
     * @brief Получение доступа к пикселю
     *
     * Возвращает константуню ссылку на пиксель с координатами (x, y)
     *
     * @param[in] x Столбец
     * @param[in] y Столбец
     *
     * @return Константная cсылка на пиксель
     */
    const Pixel& AccessPixel(const size_t x, const size_t y) const;

    /**
     * @brief Получение доступа к пикселям
     *
     * Возвращает указатель на начало внутреннего массива с пикселями. Строки изображения лежат
     * подряд в порядке сверху вниз, пиксели в строке лежат подряд слева направо
     *
     * @return Указатель на начало массива
     */
    Pixel* AccessData();

    /**
     * @brief Получение доступа к пикселям
     *
     * Возвращает указатель на начало внутреннего массива с пикселями. Строки изображения лежат
     * подряд в порядке сверху вниз, пиксели в строке лежат подряд слева направо. Данные доступны
     * только для чтения
     *
     * @return Указатель на начало массива
     */
    const Pixel* AccessData() const;

private:
    size_t width_;
    std::vector<Pixel> image_;
};
};  // namespace renderer
