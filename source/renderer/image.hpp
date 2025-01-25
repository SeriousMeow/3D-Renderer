/**
 * @file
 * @brief Изображение
 */

#pragma once

#include <vector>

namespace renderer {

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
     * Каждая компонента цвета имеет диапазон от 0.0 до 1.0
     */
    struct Pixel {
        float r;
        float g;
        float b;
    };

    Image() = delete;

    /**
     * @brief Создание изображения
     *
     * @param[in] width Ширина изображения
     * @param[in] height Высота изображения
     *
     * @todo Поставить защиту на ширину и высоту
     */
    Image(const size_t width, const size_t height);

    /**
     * @brief Создание изображения
     *
     * @param[in] other Изображение для копирования
     */
    Image(const Image& other);

    /**
     * @brief Создание изображения
     *
     * @param[in] other Изображение для перемещения
     */
    Image(Image&& other);

    /**
     * @brief Оператор присвоения
     *
     * @param[in] other Изображение для копирования
     */
    Image& operator=(const Image& other);

    /**
     * @brief Оператор присвоения
     *
     * @param[in] other Изображение для перемещения
     */
    Image& operator=(Image&& other);

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
     * @brief Изменение пикселя
     *
     * Задает новое значение для пикселя с координатами (x, y)
     *
     * @param[in] x Столбец
     * @param[in] y Строка
     * @param[in] new_pixel Новое значение пикселя
     */
    void SetPixel(const size_t x, const size_t y, const Pixel& new_pixel);

    /**
     * @brief Получение пикселя
     *
     * Возвращает значение пикселя с координатами (x, y)
     *
     * @param[in] x Столбец
     * @param[in] y Строка
     *
     * @return Значение пикселя в позиции
     */
    Pixel GetPixel(const size_t x, const size_t y);

private:
    size_t width_;
    /**
     * @todo удалить лишний параметр
     */
    size_t height_;
    std::vector<Pixel> image_;
};
};  // namespace renderer
