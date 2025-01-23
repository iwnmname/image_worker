#ifndef FILTERS_H
#define FILTERS_H

#include <algorithm>
#include <memory>
#include <cmath>
#include <stdexcept>
#include "../Image/image.h"
#include "../Parser/parser.h"

namespace filters {
class Filter {
public:
    Filter() = default;
    virtual ~Filter() = default;
    virtual Image Apply(const Image& image) const = 0;

protected:
    template <class T>
    std::vector<T> ApplyMatrix(const std::vector<std::vector<T>>& matrix, const Image& image, size_t x, size_t y) const;
};

class Negative : public Filter {
public:
    Image Apply(const Image& image) const override;
};

class Sharpening : public Filter {
public:
    Image Apply(const Image& image) const override;
};

class GrayScale : public Filter {
public:
    Image Apply(const Image& image) const override;
};

class Crop : public Filter {
public:
    explicit Crop(size_t new_width, size_t new_height);
    Image Apply(const Image& image) const override;

private:
    size_t width_;
    size_t height_;
};

class Edge : public Filter {
public:
    explicit Edge(double threshold);
    Image Apply(const Image& image) const override;

private:
    double threshold_;
};

class GaussianBlur : public Filter {
public:
    explicit GaussianBlur(double sigma);
    std::vector<double> GetMatrix() const;
    Image Apply(const Image& image) const override;

private:
    double sigma_;
};
}  // namespace filters

#endif  // FILTERS_H