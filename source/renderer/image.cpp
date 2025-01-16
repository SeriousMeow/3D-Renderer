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

std::vector<renderer::Image::Pixel>& renderer::Image::GetImageReference() {
    return image_;
}
