#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <stdexcept>
#include "color.h"
#include "../utils.h"

class Image {
public:
    Image() = default;
    Image(size_t width, size_t height);
    explicit Image(const std::vector<std::vector<Color>>& data);
    Image(const Image& other);
    ~Image() = default;
    size_t GetWidth() const;
    size_t GetHeight() const;
    Color& GetColor(size_t i, size_t j);
    std::vector<std::vector<Color>>& GetDataNonConst();
    const Color& GetColor(size_t i, size_t j) const;
    const std::vector<std::vector<Color>>& GetData() const;

private:
    size_t width_ = 0;
    size_t height_ = 0;
    std::vector<std::vector<Color>> data_;
};

#endif  // IMAGE_H