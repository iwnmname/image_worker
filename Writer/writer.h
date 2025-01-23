#ifndef WRITER_H
#define WRITER_H
#include <string>
#include "../Image/image.h"
#include "../utils.h"
#include "../Reader/reader.h"
#include <fstream>
namespace reading_and_writing {
class Writer {
public:
    static void WriteFile(const std::string& file_name, const Image& image);
    static void WriteByte(std::ostream& out_file, uint8_t byte_value);
    static void WriteValue(std::ostream& out_file, uint32_t value);
    static void WriteZero(std::ostream& out_file, size_t size);
};
}  // namespace reading_and_writing
#endif  // WRITER_H