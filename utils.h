#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <iostream>

namespace image::utils {
const int BMP_HEADER_SIZE = 14;
const int DIB_HEADER_SIZE = 40;
const int PADDING_BYTES = 4;
const int BYTES_PER_INT = 4;
const int BYTES_PER_PIXEL = 3;
const int HEADER_WIDTH_OFFSET = 4;
const int HEADER_HEIGHT_OFFSET = 8;
const int HEADER_FILE_SIZE_OFFSET = 2;
const int PIXEL_ARRAY_OFFSET = 10;
const int FILE_FORMAT_FIRST_POSITION = 0;
const int FILE_FORMAT_SECOND_POSITION = 1;
const int INFORMATION_HEADER_SIZE_POSITION = 0;
const int COLOR_PLANES = 1;
const int COLOR_PLANES_POSITION = 12;
const int BITS_PER_PIXEL_POSITION = 14;
const int BITS_PER_BYTE = 8;
const int BITS_PER_PIXEL = 24;
const int MAX_RGB = 255;
const size_t BYTE_MASK = 0xFF;
const std::vector<int> SHIFT_BITS = {8, 16, 24};
const std::vector<double> GRAYSCALE_NUMBER = {0.299, 0.587, 0.114};
const std::vector<char> HEADER_SIGNATURE = {'B', 'M'};
}  // namespace image::utils

#endif  // UTILS_H