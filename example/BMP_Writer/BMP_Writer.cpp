#include "BMP_Writer.hpp"

#include <cassert>
#include <cstdint>
#include <fstream>

namespace {
/**
 * @brief Константы для создания BMP файла
 */
constexpr uint16_t kFileType = 0x4D42;
constexpr uint16_t kBitCount = 24;
constexpr uint8_t kRowPadding = 4;
constexpr uint32_t kRedMask = 0xFF0000;
constexpr uint32_t kGreenMask = 0x00FF00;
constexpr uint32_t kBlueMask = 0x0000FF;
constexpr uint32_t kBitsInByte = 8;
constexpr uint32_t kCompressionTypeUncopressed = 0;

/**
 * @brief Заголовки BMP файлов
 */
#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t file_type{kFileType};
    uint32_t file_size{0};
    uint16_t reserved1{0};
    uint16_t reserved2{0};
    uint32_t offset_data{0};
};
#pragma pack(pop)

#pragma pack(push, 1)
struct BMPInfoHeader {
    uint32_t size{sizeof(BMPInfoHeader)};
    int32_t width{0};
    int32_t height{0};
    uint16_t planes{1};
    uint16_t bit_count{kBitCount};
    uint32_t compression{0};
    uint32_t size_image{0};
    int32_t x_pixels_per_meter{0};
    int32_t y_pixels_per_meter{0};
    uint32_t colors_used{0};
    uint32_t colors_important{0};
};
#pragma pack(pop)

/**
 * @brief Конвертация пикселя в битовое представление
 */
uint32_t PixelToBits(const renderer::Image::Pixel& pixel) {
    uint32_t bits{0};
    int32_t bits_per_channel = kBitCount / 3;
    int32_t max_bit_value = (1 << bits_per_channel) - 1;
    uint16_t blue_bits = static_cast<uint16_t>(pixel.b * max_bit_value);
    uint16_t green_bits = static_cast<uint16_t>(pixel.g * max_bit_value);
    uint16_t red_bits = static_cast<uint16_t>(pixel.r * max_bit_value);
    bits = bits | (red_bits << (bits_per_channel * 2));
    bits = bits | (green_bits << (bits_per_channel));
    bits = bits | blue_bits;
    return bits;
}
}  // namespace

void SaveToBmp(const renderer::Image& image, const std::string& path) {
    {
        assert((image.GetWidth() > 0) and "SaveToBmp: ширина должна быть больше 0");
        assert((image.GetHeight() > 0) and "SaveToBmp: высота должна быть больше 0");
    }
    std::ofstream ouput_image{path, std::ios_base::binary};
    if (not ouput_image) {
        throw std::runtime_error("SaveToBmp: не удалось открыть указанный файл для записи");
    }
    BMPInfoHeader info_header;
    BMPFileHeader file_header;

    file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

    info_header.height = static_cast<int32_t>(image.GetHeight());
    info_header.width = static_cast<int32_t>(image.GetWidth());

    size_t full_row_size =
        ((info_header.width * info_header.bit_count + kRowPadding * kBitsInByte - 1) /
         (kRowPadding * kBitsInByte)) *
        kRowPadding;
    file_header.file_size = file_header.offset_data + full_row_size * info_header.height;

    ouput_image.write(reinterpret_cast<const char*>(&file_header), sizeof(file_header));
    ouput_image.write(reinterpret_cast<const char*>(&info_header), sizeof(info_header));

    size_t row_size = (info_header.width * info_header.bit_count) / kBitsInByte;
    uint32_t row_padding = (kRowPadding - row_size % kRowPadding) % kRowPadding;

    for (int32_t row = 0; row < info_header.height; ++row) {
        for (int32_t column = 0; column < info_header.width; ++column) {
            const renderer::Image::Pixel current_pixel =
                image.GetPixel(column, info_header.height - 1 - row);  // BMP пишется снизу вверх
            const uint32_t pixel_as_bits = PixelToBits(current_pixel);
            ouput_image.write(reinterpret_cast<const char*>(&pixel_as_bits), 3);
        }
        uint32_t padding{0};
        if (row_padding > 0) {
            ouput_image.write(reinterpret_cast<const char*>(&padding), row_padding);
        }
    }
}
