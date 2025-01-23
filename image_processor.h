#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include "Filter/filters.h"
#include "Image/image.h"
#include "Parser/parser.h"
#include "Reader/reader.h"
#include "Writer/writer.h"

Image GetImage(const std::string& path);
Image ApplyFilter(Image& image, Parser& info);

#endif  // IMAGE_PROCESSOR_H
