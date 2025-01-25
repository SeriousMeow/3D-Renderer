#include "renderer/image.hpp"

#include <cassert>

renderer::Image::Image(const size_t width, const size_t height) : width_{width}, height_{height} {
    {
        assert((width > 0) and "Image: ширина должна быть больше 0");
        assert((height > 0) and "Image: высота должна быть больше 0");
    }
    image_.resize(width * height);
}

renderer::Image::Image(const Image& other)
    : width_{other.width_}, height_{other.height_}, image_{other.image_} {
}

renderer::Image::Image(Image&& other)
    : width_{std::move(other.width_)},
      height_{std::move(other.height_)},
      image_{std::move(other.image_)} {
}

renderer::Image& renderer::Image::operator=(const Image& other) {
    width_ = other.width_;
    height_ = other.height_;
    image_ = other.image_;
    return *this;
}

renderer::Image& renderer::Image::operator=(Image&& other) {
    if (this != &other) {
        width_ = std::move(other.width_);
        height_ = std::move(other.height_);
        image_ = std::move(other.image_);
    }
    return *this;
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
