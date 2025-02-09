#include "color.h"

GUI90_Color GUI90_InterpolateColors(GUI90_Color color0, GUI90_Color color1, uint32_t t) {
    uint32_t r0 = (color0 >> 16) & 0xFF;
    uint32_t g0 = (color0 >> 8) & 0xFF;
    uint32_t b0 = (color0 >> 0) & 0xFF;

    uint32_t r1 = (color1 >> 16) & 0xFF;
    uint32_t g1 = (color1 >> 8) & 0xFF;
    uint32_t b1 = (color1 >> 0) & 0xFF;

    uint32_t r = (r0 * (255 - t) + r1 * t) / 255;
    uint32_t g = (g0 * (255 - t) + g1 * t) / 255;
    uint32_t b = (b0 * (255 - t) + b1 * t) / 255;

    return GUI90_RGB(r, g, b);
}
