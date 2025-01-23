#include "image.h"

Image::Image(size_t width, size_t height) : width_(width), height_(height){};

Image::Image(const std::vector<std::vector<Color>>& data) : width_(data[0].size()), height_(data.size()), data_(data){};

Image::Image(const Image& other) : width_(other.width_), height_(other.height_), data_(other.data_){};

size_t Image::GetWidth() const {
    return width_;
}

size_t Image::GetHeight() const {
    return height_;
}

const std::vector<std::vector<Color>>& Image::GetData() const {
    return data_;
}

std::vector<std::vector<Color>>& Image::GetDataNonConst() {
    return data_;
}

Color& Image::GetColor(size_t i, size_t j) {
    if (i >= height_ || j >= width_) {
        throw std::invalid_argument("Incorrect height or width");
    }
    return data_[i][j];
}
const Color& Image::GetColor(size_t i, size_t j) const {
    if (i >= height_ || j >= width_) {
        throw std::invalid_argument("Incorrect height or width");
    }
    return data_[i][j];
}
