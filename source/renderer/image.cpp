#include "renderer/image.hpp"

renderer::Image::Image(const size_t width, const size_t height) : width_{width}, height_{height} {
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
