#include "renderer/image.hpp"

#include <cassert>

namespace renderer {

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

}  // namespace renderer
