/**
 * @file
 * @brief Полезные инструменты
 */

#pragma once

#include <string>

#include "renderer/object.hpp"

namespace renderer {
/**
 * @brief Полезные инструменты
 *
 * Данные инструменты могут облегчить работу с библиотекой в некоторых сценариях, но они не являются
 * основной целью разработки
 */
namespace utils {

/**
 * @brief Загрузка объекта из файла
 *
 * Загружает объект файла, распологающегося по пути path. Полный список поддерживаемых типов
 * доступен в документации библиотеки assimp. В случае ошибки загрузки возвращается пустой объект
 *
 * @param[in] path Путь до файла
 *
 * @return Объект, загруженный из файла
 */
Object LoadFile(const std::string& path);

};  // namespace utils
};  // namespace renderer
