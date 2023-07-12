#pragma once

#include <cstdint>

using Color = uint32_t;

Color packColorRgb(uint32_t r, uint32_t g, uint32_t b);

struct ColorShades {
    Color foreground;
    Color background;
    Color bevel_light;
    Color bevel_dark;
    Color border;
};

const auto YELLOW_SHADES = ColorShades{
    packColorRgb(0, 0, 0),
    packColorRgb(255, 221, 63),
    packColorRgb(255, 245, 197),
    packColorRgb(207, 117, 43),
    packColorRgb(0, 0, 0),
};

const auto GRAY_SHADES = ColorShades{
    packColorRgb(255, 255, 255),
    packColorRgb(128, 128, 128),
    packColorRgb(192, 192, 192),
    packColorRgb(80, 80, 80),
    packColorRgb(0, 0, 0),
};

const auto LEATHER_SHADES = ColorShades{
    packColorRgb(0, 0, 0),
    packColorRgb(70, 50, 40),
    packColorRgb(95, 80, 73),
    packColorRgb(54, 33, 22),
    packColorRgb(0, 0, 0),
};
