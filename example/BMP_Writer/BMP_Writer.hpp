/**
 * @file
 * @brief Создание BMP изображений
 */

#pragma once

#include <cstdint>
#include <string>
#include <vector>

/**
 * @namespace
 * @brief Создание BMP изображений
 */
namespace bmp {

namespace bmp_constants {
const uint16_t kFileType = 0x4D42;
const uint16_t kBitCount = 24;
const uint8_t kRowPadding = 4;
const uint32_t kRedMask = 0xFF0000;
const uint32_t kGreenMask = 0x00FF00;
const uint32_t kBlueMask = 0x0000FF;
const uint32_t kBitsInByte = 8;
const uint32_t kCompressionTypeUncopressed = 0;
}  // namespace bmp_constants

struct Pixel {
    double r;
    double g;
    double b;
    Pixel& operator+=(const Pixel& rhs);
    Pixel operator*(double value) const;
    void Normalization();
};

class Image {
public:
    Image(const int32_t width, const int32_t height);
    void Save(const std::string& save_path);

    const int32_t GetWidth() const;
    const int32_t GetHeight() const;

    const Pixel* const GetPixel(const int32_t x, const int32_t y) const;

    Pixel* const GetPixel(const int32_t x, const int32_t y);

private:
#pragma pack(push, 1)
    struct BMPFileHeader {
        uint16_t file_type{bmp_constants::kFileType};
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
        uint16_t bit_count{bmp_constants::kBitCount};
        uint32_t compression{0};
        uint32_t size_image{0};
        int32_t x_pixels_per_meter{0};
        int32_t y_pixels_per_meter{0};
        uint32_t colors_used{0};
        uint32_t colors_important{0};
    };
#pragma pack(pop)
    void CheckFileHeader() const;
    void CheckInfoHeader() const;
    void GenerateHeaders();
    const Pixel BitsToPixel(const uint32_t bits) const;
    const uint32_t PixelToBits(const Pixel& pixel) const;
    BMPFileHeader file_header_;
    BMPInfoHeader bmp_info_header_;
    std::vector<std::vector<Pixel>> image_;
};
};  // namespace bmp
