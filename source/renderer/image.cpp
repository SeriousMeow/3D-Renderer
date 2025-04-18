#include "renderer/image.hpp"

#include <cassert>

namespace renderer {

Color Image::Pixel::ToColor(const Image::Pixel& pixel) {
    return Color{static_cast<float>(pixel.r) / 255.0f, static_cast<float>(pixel.g) / 255.0f,
                 static_cast<float>(pixel.b) / 255.0f};
}

Image::Image(const Width width, const Height height) : width_{width} {
    image_.resize(width_ * static_cast<size_t>(height));
}

size_t Image::GetWidth() const {
    return width_;
}

size_t Image::GetHeight() const {
    if (width_ == 0) {
        return image_.size();
    }
    return image_.size() / width_;
}

Image::Pixel& Image::AccessPixel(const size_t x, const size_t y) {
    {
        assert((x < GetWidth()) and "AccessPixel: координата x должна быть меньше ширины");
        assert((y < GetHeight()) and "AccessPixel: координата y должна быть меньше высоты");
    }
    return image_[x + y * width_];
}

const Image::Pixel& Image::AccessPixel(const size_t x, const size_t y) const {
    {
        assert((x < GetWidth()) and "AccessPixel: координата x должна быть меньше ширины");
        assert((y < GetHeight()) and "AccessPixel: координата y должна быть меньше высоты");
    }
    return image_[x + y * width_];
}

Image::Pixel* Image::AccessData() {
    return image_.data();
}

const Image::Pixel* Image::AccessData() const {
    return image_.data();
}

}  // namespace renderer
