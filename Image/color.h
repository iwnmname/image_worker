#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

struct Color {
    uint8_t blue = 0;
    uint8_t green = 0;
    uint8_t red = 0;
    Color() = default;
    Color(uint8_t blue_in, uint8_t green_in, uint8_t red_in);

    void SetVals(uint8_t blue_in, uint8_t green_in, uint8_t red_in);
};

#endif  // COLOR_H