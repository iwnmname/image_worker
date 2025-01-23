#ifndef READER_H
#define READER_H

#include <string>
#include <stdexcept>
#include "../Image/image.h"

namespace reading_and_writing {
class Reader {
private:
    std::string path_;
    size_t BytesToRead(unsigned char* bytes);

public:
    explicit Reader(const std::string& path);
    Image Read();
};
size_t GetPaddingSize(size_t width);
}  // namespace reading_and_writing
#endif  // READER_H