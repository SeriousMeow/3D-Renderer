#include "renderer/image.hpp"

#include <cassert>

renderer::Image::Image(const Width width, const Height height) : width_{width} {
    {
        assert((width > 0) and "Image: ширина должна быть больше 0");
        assert((height > 0) and "Image: высота должна быть больше 0");
    }
    image_.resize(width_ * static_cast<size_t>(height));
}

renderer::Image::Image(const Image& other) : width_{other.width_}, image_{other.image_} {
}

renderer::Image::Image(Image&& other)
    : width_{std::move(other.width_)}, image_{std::move(other.image_)} {
}

renderer::Image& renderer::Image::operator=(const Image& other) {
    width_ = other.width_;
    image_ = other.image_;
    return *this;
}

renderer::Image& renderer::Image::operator=(Image&& other) {
    if (this != &other) {
        width_ = std::move(other.width_);
        image_ = std::move(other.image_);
    }
    return *this;
}

size_t renderer::Image::GetWidth() const {
    return width_;
}

size_t renderer::Image::GetHeight() const {
    return image_.size() / width_;
}

void renderer::Image::SetPixel(const size_t x, const size_t y,
                               const renderer::Image::Pixel& new_pixel) {
    image_[x + y * width_] = new_pixel;
}

renderer::Image::Pixel renderer::Image::GetPixel(const size_t x, const size_t y) {
    return image_[x + y * width_];
}
