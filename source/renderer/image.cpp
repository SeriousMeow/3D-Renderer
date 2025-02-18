#include "renderer/image.hpp"

#include <cassert>

namespace renderer {

Image::Image(const Width width, const Height height) : width_{width} {
    {
        assert((width > 0) and "Image: ширина должна быть больше 0");
        assert((height > 0) and "Image: высота должна быть больше 0");
    }
    image_.resize(width_ * static_cast<size_t>(height));
}

size_t Image::GetWidth() const {
    return width_;
}

size_t Image::GetHeight() const {
    return image_.size() / width_;
}

void Image::SetPixel(const size_t x, const size_t y, const Pixel& new_pixel) {
    {
        assert((0 <= new_pixel.r and new_pixel.r <= 1) and
               "SetPixel: компонента r пикселя должна лежать в диапазоне от 0 до 1");
        assert((0 <= new_pixel.g and new_pixel.g <= 1) and
               "SetPixel: компонента g пикселя должна лежать в диапазоне от 0 до 1");
        assert((0 <= new_pixel.b and new_pixel.b <= 1) and
               "SetPixel: компонента b пикселя должна лежать в диапазоне от 0 до 1");
    }
    image_[x + y * width_] = new_pixel;
}

Image::Pixel Image::GetPixel(const size_t x, const size_t y) const {
    return image_[x + y * width_];
}
}  // namespace renderer
