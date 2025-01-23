#include "writer.h"
#include <utility>

void reading_and_writing::Writer::WriteByte(std::ostream& out_file, uint8_t byte_value) {
    out_file.write(reinterpret_cast<char*>(&byte_value), 1);
}

void reading_and_writing::Writer::WriteValue(std::ostream& out_file, uint32_t value) {
    for (size_t i = 0; i < image::utils::BYTES_PER_INT; ++i) {
        WriteByte(out_file, (value >> i * image::utils::BITS_PER_BYTE) & image::utils::BYTE_MASK);
    }
}

void reading_and_writing::Writer::WriteZero(std::ostream& out_file, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        WriteByte(out_file, 0);
    }
}

void reading_and_writing::Writer::WriteFile(const std::string& file_name, const Image& img) {
    std::ofstream out_file(file_name, std::ios::binary);
    WriteByte(out_file, image::utils::HEADER_SIGNATURE[0]);
    WriteByte(out_file, image::utils::HEADER_SIGNATURE[1]);
    auto header_size = image::utils::BMP_HEADER_SIZE + image::utils::DIB_HEADER_SIZE;
    auto file_size = image::utils::BMP_HEADER_SIZE + image::utils::DIB_HEADER_SIZE +
                     img.GetHeight() * img.GetWidth() * image::utils::BYTES_PER_PIXEL +
                     reading_and_writing::GetPaddingSize(img.GetWidth()) * img.GetHeight();
    WriteValue(out_file, file_size);
    WriteZero(out_file, 4);
    WriteValue(out_file, header_size);
    WriteValue(out_file, image::utils::DIB_HEADER_SIZE);
    WriteValue(out_file, img.GetWidth());
    WriteValue(out_file, img.GetHeight());
    WriteByte(out_file, image::utils::COLOR_PLANES);
    WriteZero(out_file, image::utils::COLOR_PLANES);
    WriteByte(out_file, image::utils::BITS_PER_PIXEL);
    WriteZero(out_file, image::utils::COLOR_PLANES);
    WriteZero(out_file, image::utils::BITS_PER_PIXEL);
    for (size_t i = 0; i < img.GetHeight(); ++i) {
        for (size_t j = 0; j < img.GetWidth(); ++j) {
            WriteByte(out_file, img.GetColor(i, j).blue);
            WriteByte(out_file, img.GetColor(i, j).green);
            WriteByte(out_file, img.GetColor(i, j).red);
        }
        WriteZero(out_file, reading_and_writing::GetPaddingSize(img.GetWidth()));
    }
    out_file.close();
}