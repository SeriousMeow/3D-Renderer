#include "renderer/image.hpp"

#include <cassert>

renderer::Image::Image(const size_t width, const size_t height) : width_{width}, height_{height} {
    {
        assert((width > 0) and "Image: ширина должна быть больше 0");
        assert((height > 0) and "Image: высота должна быть больше 0");
    }
    image_.resize(width * height);
}

size_t renderer::Image::GetWidth() const {
    return width_;
}

size_t renderer::Image::GetHeight() const {
    return height_;
}

void renderer::Image::SetPixel(const size_t x, const size_t y,
                               const renderer::Image::Pixel& new_pixel) {
    image_[x + y * width_] = new_pixel;
}

renderer::Image::Pixel renderer::Image::GetPixel(const size_t x, const size_t y) {
    return image_[x + y * width_];
}
