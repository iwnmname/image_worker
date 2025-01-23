#include "filters.h"

Image filters::Negative::Apply(const Image &image) const {
    std::vector<std::vector<Color>> new_image_data;
    for (size_t i = 0; i < image.GetHeight(); ++i) {
        std::vector<Color> row(image.GetWidth());
        for (size_t j = 0; j < image.GetWidth(); ++j) {
            row[j] = {static_cast<uint8_t>(image::utils::MAX_RGB - image.GetColor(i, j).blue),
                      static_cast<uint8_t>(image::utils::MAX_RGB - image.GetColor(i, j).green),
                      static_cast<uint8_t>(image::utils::MAX_RGB - image.GetColor(i, j).red)};
        }
        new_image_data.push_back(row);
    }
    return Image(new_image_data);
}

template <typename T>
std::vector<T> filters::Filter::ApplyMatrix(const std::vector<std::vector<T>> &matrix, const Image &image, size_t x,
                                            size_t y) const {
    T red = 0;
    T green = 0;
    T blue = 0;
    size_t height = image.GetHeight();
    size_t width = image.GetWidth();

    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix[i].size(); ++j) {
            size_t x_index = x + i;
            size_t y_index = y + j;
            size_t clamped_x = 0;
            size_t clamped_y = 0;
            if (x_index == 0) {
                clamped_x = std::clamp(x_index, static_cast<size_t>(0), height - 1);
            } else {
                clamped_x = std::clamp(x - 1 + i, static_cast<size_t>(0), height - 1);
            }
            if (y_index == 0) {
                clamped_y = std::clamp(y_index, static_cast<size_t>(0), width - 1);
            } else {
                clamped_y = std::clamp(y - 1 + j, static_cast<size_t>(0), width - 1);
            }
            const auto &color = image.GetColor(clamped_x, clamped_y);
            red += color.red * matrix[i][j];
            green += color.green * matrix[i][j];
            blue += color.blue * matrix[i][j];
        }
    }
    return {blue, green, red};
}

uint8_t GetCorrectColor(int color, const uint8_t max_color = 255) {
    if (color < 0) {
        return 0;
    } else if (color > max_color) {
        return max_color;
    }
    return static_cast<uint8_t>(color);
}

Image filters::Sharpening::Apply(const Image &image) const {
    std::vector<std::vector<Color>> new_image_data;
    const std::vector<std::vector<int>> matrix = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
    for (size_t i = 0; i < image.GetHeight(); ++i) {
        std::vector<Color> row(image.GetWidth());
        for (size_t j = 0; j < image.GetWidth(); ++j) {
            auto pix_color = ApplyMatrix<int>(matrix, image, i, j);
            row[j] = {GetCorrectColor(pix_color[0]), GetCorrectColor(pix_color[1]), GetCorrectColor(pix_color[2])};
        }
        new_image_data.push_back(row);
    }
    return Image(new_image_data);
}

Image filters::GrayScale::Apply(const Image &image) const {
    std::vector<std::vector<Color>> new_image_data;
    for (size_t i = 0; i < image.GetHeight(); ++i) {
        std::vector<Color> row(image.GetWidth());
        for (size_t j = 0; j < image.GetWidth(); ++j) {
            auto gray_pixel = (image::utils::GRAYSCALE_NUMBER[0] * image.GetColor(i, j).red) +
                              (image::utils::GRAYSCALE_NUMBER[1] * image.GetColor(i, j).green) +
                              (image::utils::GRAYSCALE_NUMBER[2] * image.GetColor(i, j).blue);
            row[j] = {static_cast<uint8_t>(gray_pixel), static_cast<uint8_t>(gray_pixel),
                      static_cast<uint8_t>(gray_pixel)};
        }
        new_image_data.push_back(row);
    }
    return Image(new_image_data);
}

filters::Crop::Crop(size_t new_width, size_t new_height) : width_(new_width), height_(new_height){};

Image filters::Crop::Apply(const Image &image) const {
    Image new_image = image;
    std::vector<std::vector<Color>> new_data;
    std::reverse(new_image.GetDataNonConst().begin(), new_image.GetDataNonConst().end());
    size_t new_height = std::min(image.GetHeight(), height_);
    size_t new_width = std::min(image.GetWidth(), width_);
    new_data.assign(new_height, std::vector<Color>(new_width));
    for (size_t i = 0; i < new_height; ++i) {
        for (size_t j = 0; j < new_width; ++j) {
            new_data[i][j] = new_image.GetColor(i, j);
        }
    }
    std::reverse(new_data.begin(), new_data.end());
    return Image(new_data);
}

filters::Edge::Edge(double threshold) : threshold_(threshold){};

Image filters::Edge::Apply(const Image &image) const {
    std::vector<std::vector<Color>> new_image_data;
    filters::GrayScale gs;
    Image image_gs = gs.Apply(image);
    const std::vector<std::vector<int>> matrix = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
    for (size_t i = 0; i < image_gs.GetHeight(); ++i) {
        std::vector<Color> row(image_gs.GetWidth());
        for (size_t j = 0; j < image_gs.GetWidth(); ++j) {
            auto pix_color = ApplyMatrix<int>(matrix, image_gs, i, j);
            const double pix_value = static_cast<double>(pix_color[0]) / image::utils::MAX_RGB;
            if (pix_value <= threshold_) {
                row[j] = {0, 0, 0};
            } else {
                row[j] = {image::utils::MAX_RGB, image::utils::MAX_RGB, image::utils::MAX_RGB};
            }
        }
        new_image_data.push_back(row);
    }
    return Image(new_image_data);
}

filters::GaussianBlur::GaussianBlur(double sigma) : sigma_(sigma){};

std::vector<double> filters::GaussianBlur::GetMatrix() const {
    int dim = static_cast<int>(std::ceil(sigma_ * 3)) * 2 + 1;
    int half_size = dim / 2;
    std::vector<double> gauss_matrix(dim);

    double sum = 0;
    for (int x = -half_size; x <= half_size; ++x) {
        double e_num = -(static_cast<double>(x) * static_cast<double>(x));
        double e_denum = (2 * sigma_ * sigma_);
        double val = std::exp(e_num / e_denum);
        gauss_matrix[x + half_size] = val;
        sum += val;
    }

    for (int i = 0; i < dim; ++i) {
        gauss_matrix[i] /= sum;
    }
    return gauss_matrix;
}

Image filters::GaussianBlur::Apply(const Image &image) const {
    std::vector<double> gauss_matrix = GetMatrix();
    std::vector<std::vector<Color>> horiz_image_data(image.GetHeight(), std::vector<Color>(image.GetWidth()));
    std::vector<std::vector<Color>> new_image_data(image.GetHeight(), std::vector<Color>(image.GetWidth()));
    int half_size = static_cast<int>(gauss_matrix.size()) / 2;

    for (size_t i = 0; i < image.GetHeight(); ++i) {
        for (size_t j = 0; j < image.GetWidth(); ++j) {
            double b = 0;
            double g = 0;
            double r = 0;
            for (int k = -half_size; k <= half_size; ++k) {
                int col = std::clamp(static_cast<int>(j) + k, 0, static_cast<int>(image.GetWidth()) - 1);
                Color pixels = image.GetColor(i, col);
                b += static_cast<double>(pixels.blue) * gauss_matrix[k + half_size];
                g += static_cast<double>(pixels.green) * gauss_matrix[k + half_size];
                r += static_cast<double>(pixels.red) * gauss_matrix[k + half_size];
            }
            horiz_image_data[i][j].SetVals(static_cast<uint8_t>(b), static_cast<uint8_t>(g), static_cast<uint8_t>(r));
        }
    }

    for (size_t i = 0; i < image.GetHeight(); ++i) {
        for (size_t j = 0; j < image.GetWidth(); ++j) {
            double b = 0;
            double g = 0;
            double r = 0;
            for (int k = -half_size; k <= half_size; ++k) {
                int m_row = std::clamp(static_cast<int>(i) + k, 0, static_cast<int>(image.GetHeight()) - 1);
                Color pixels = horiz_image_data[m_row][j];
                b += static_cast<double>(pixels.blue) * gauss_matrix[half_size - k];
                g += static_cast<double>(pixels.green) * gauss_matrix[half_size - k];
                r += static_cast<double>(pixels.red) * gauss_matrix[half_size - k];
            }
            new_image_data[i][j].SetVals(static_cast<uint8_t>(b), static_cast<uint8_t>(g), static_cast<uint8_t>(r));
        }
    }

    return Image(new_image_data);
}