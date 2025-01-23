#include "image_processor.h"

Image GetImage(const std::string& path) {
    reading_and_writing::Reader reader(path);
    Image image = reader.Read();
    return image;
}

Image ApplyFilter(Image& image, Parser& info) {
    for (const std::string& i : info.filters_) {
        if (i == "-crop") {
            if (!info.filters_args_[i].empty()) {
                size_t width = std::stoul(info.filters_args_[i][0]);
                size_t height = std::stoul(info.filters_args_[i][1]);
                filters::Crop crop(width, height);
                image = crop.Apply(image);
            } else {
                throw std::invalid_argument("Invalid arguments");
            }
        }
        if (i == "-gs") {
            filters::GrayScale gs;
            image = gs.Apply(image);
        }
        if (i == "-neg") {
            filters::Negative neg;
            image = neg.Apply(image);
        }
        if (i == "-sharp") {
            filters::Sharpening sharp;
            image = sharp.Apply(image);
        }
        if (i == "-edge") {
            if (!info.filters_args_[i].empty()) {
                double threshold = std::stod(info.filters_args_[i][0]);
                filters::Edge edge(threshold);
                image = edge.Apply(image);
            } else {
                throw std::invalid_argument("Invalid argument");
            }
        }
        if (i == "-blur") {
            if (!info.filters_args_[i].empty()) {
                double sigma = std::stod(info.filters_args_[i][0]);
                filters::GaussianBlur gb(sigma);
                image = gb.Apply(image);
            } else {
                throw std::invalid_argument("Invalid arguments");
            }
        }
    }
    return image;
}

int main(int argc, char** argv) {
    Parser arguments = Parse(argc, argv);
    Image image = GetImage(arguments.file_in_);
    image = ApplyFilter(image, arguments);
    reading_and_writing::Writer::WriteFile(arguments.file_out_, image);
    return 0;
}