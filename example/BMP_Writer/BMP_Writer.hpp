/**
 * @file
 * @brief Запись renderer::Image в BMP формате
 */

#pragma once

#include <renderer/image.hpp>
#include <string>

/**
 * @brief Сохранение изображения в BMP формате
 *
 * Сохраняет переданное изображение в файл, указанный в path. Итоговое изображение имеет формат BMP
 *
 * @param[in] image Изображение для записи
 * @param[in] path Путь для сохранения
 *
 * @bug Может не работать для систем с big endian
 * @todo Сделать универсальную работу с endianness
 */
void SaveToBmp(const renderer::Image& image, const std::string& path);
