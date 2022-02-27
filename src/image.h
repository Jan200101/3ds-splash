#ifndef IMAGE_H
#define IMAGE_H

#include <stddef.h>
#include <stdint.h>

size_t bin_to_rgba(uint8_t**, size_t);
size_t rgba_to_bin(uint8_t**, size_t);

size_t png_to_rgba(uint8_t**, size_t);
size_t rgba_to_png(uint8_t**, size_t);

#endif