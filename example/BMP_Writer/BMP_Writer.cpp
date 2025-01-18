#include "BMP_Writer.hpp"

#include <algorithm>
#include <cassert>
#include <fstream>

bmp::Pixel& bmp::Pixel::operator+=(const Pixel& rhs) {
    r += rhs.r;
    g += rhs.g;
    b += rhs.b;
    return *this;
}

bmp::Pixel bmp::Pixel::operator*(double value) const {
    return {r * value, g * value, b * value};
}

void bmp::Pixel::Normalization() {
    const double zero{0};
    const double one{1};
    r = std::min(one, std::max(zero, r));
    g = std::min(one, std::max(zero, g));
    b = std::min(one, std::max(zero, b));
}

bmp::Image::Image(const int32_t width, const int32_t heiht) {
    {
        assert((width > 0) and "Image: ширина должна быть больше 0");
        assert((heiht > 0) and "Image: высота должна быть больше 0");
    }
    bmp_info_header_.width = width;
    bmp_info_header_.height = heiht;
    image_ = std::vector<std::vector<bmp::Pixel>>(heiht, std::vector<bmp::Pixel>(width));
}

void bmp::Image::Save(const std::string& save_path) {
    std::ofstream image{save_path, std::ios_base::binary};
    if (!image) {
        throw std::runtime_error("Unable to open the output image file");
    }
    GenerateHeaders();
    image.write(reinterpret_cast<const char*>(&file_header_), sizeof(file_header_));
    image.write(reinterpret_cast<const char*>(&bmp_info_header_), sizeof(bmp_info_header_));

    size_t row_size =
        (bmp_info_header_.width * bmp_info_header_.bit_count) / bmp_constants::kBitsInByte;
    uint32_t row_padding = (bmp_constants::kRowPadding - row_size % bmp_constants::kRowPadding) %
                           bmp_constants::kRowPadding;
    for (int32_t row = 0; row < bmp_info_header_.height; ++row) {
        for (int32_t column = 0; column < bmp_info_header_.width; ++column) {
            const Pixel current_pixel = image_[bmp_info_header_.height - 1 - row][column];
            const uint32_t pixel_bit = PixelToBits(current_pixel);
            image.write(reinterpret_cast<const char*>(&pixel_bit), 3);
        }
        uint32_t padding{0};
        if (row_padding > 0) {
            image.write(reinterpret_cast<const char*>(&padding), row_padding);
        }
    }
}

void bmp::Image::CheckFileHeader() const {
    if (file_header_.file_type != bmp_constants::kFileType) {
        throw std::runtime_error("Wrong input file format");
    }
}

void bmp::Image::CheckInfoHeader() const {
    if (bmp_info_header_.width < 0) {
        throw std::runtime_error("Input file is corrupted");
    }
    if (bmp_info_header_.height < 0) {
        throw std::runtime_error(
            "This program can treat only BMP images with the origin in the bottom left corner!");
    }
    if (bmp_info_header_.bit_count != bmp_constants::kBitCount) {
        throw std::runtime_error("This program supports only " +
                                 std::to_string(bmp_constants::kBitCount) + " bits color depht");
    }
    if (bmp_info_header_.compression != bmp_constants::kCompressionTypeUncopressed) {
        throw std::runtime_error("This program supports only uncopressed files");
    }
}

void bmp::Image::GenerateHeaders() {
    file_header_ = BMPFileHeader{};
    file_header_.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

    const int32_t x_pixels_per_meter = bmp_info_header_.x_pixels_per_meter;
    const int32_t y_pixels_per_meter = bmp_info_header_.y_pixels_per_meter;
    bmp_info_header_ = BMPInfoHeader{};
    bmp_info_header_.x_pixels_per_meter = x_pixels_per_meter;
    bmp_info_header_.y_pixels_per_meter = y_pixels_per_meter;

    bmp_info_header_.height = static_cast<int32_t>(image_.size());
    if (!image_.empty()) {
        bmp_info_header_.width = static_cast<int32_t>(image_[0].size());
    } else {
        bmp_info_header_.width = 0;
    }
    size_t full_row_size = ((bmp_info_header_.width * bmp_info_header_.bit_count +
                             bmp_constants::kRowPadding * bmp_constants::kBitsInByte - 1) /
                            (bmp_constants::kRowPadding * bmp_constants::kBitsInByte)) *
                           bmp_constants::kRowPadding;
    file_header_.file_size =
        file_header_.offset_data + full_row_size * std::abs(bmp_info_header_.height);
}

const bmp::Pixel bmp::Image::BitsToPixel(const uint32_t bits) const {
    int32_t bits_per_channel = bmp_constants::kBitCount / 3;
    uint16_t red_bits = (bits & bmp_constants::kRedMask) >> (bits_per_channel * 2);
    uint16_t green_bits = (bits & bmp_constants::kGreenMask) >> (bits_per_channel);
    uint16_t blue_bits = (bits & bmp_constants::kBlueMask);
    int32_t max_bit_value = (1 << bits_per_channel) - 1;
    double red = static_cast<double>(red_bits) / max_bit_value;
    double green = static_cast<double>(green_bits) / max_bit_value;
    double blue = static_cast<double>(blue_bits) / max_bit_value;
    return Pixel{red, green, blue};
}

const uint32_t bmp::Image::PixelToBits(const Pixel& pixel) const {
    uint32_t bits{0};
    int32_t bits_per_channel = bmp_constants::kBitCount / 3;
    int32_t max_bit_value = (1 << bits_per_channel) - 1;
    uint16_t blue_bits = static_cast<uint16_t>(pixel.b * max_bit_value);
    uint16_t green_bits = static_cast<uint16_t>(pixel.g * max_bit_value);
    uint16_t red_bits = static_cast<uint16_t>(pixel.r * max_bit_value);
    bits = bits | (red_bits << (bits_per_channel * 2));
    bits = bits | (green_bits << (bits_per_channel));
    bits = bits | blue_bits;
    return bits;
}

const int32_t bmp::Image::GetWidth() const {
    if (image_.empty()) {
        return 0;
    }
    return static_cast<int32_t>(image_[0].size());
}

const int32_t bmp::Image::GetHeight() const {
    return static_cast<int32_t>(image_.size());
}

const bmp::Pixel* const bmp::Image::GetPixel(const int32_t x, const int32_t y) const {
    if (x > bmp_info_header_.width || y > bmp_info_header_.height) {
        throw std::runtime_error("Image out of range");
        return nullptr;
    }
    return &image_[y][x];
}

bmp::Pixel* const bmp::Image::GetPixel(const int32_t x, const int32_t y) {
    if (x > bmp_info_header_.width || y > bmp_info_header_.height) {
        throw std::runtime_error("Image out of range");
        return nullptr;
    }
    return &image_[y][x];
}
