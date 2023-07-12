#pragma once

#include <cstdint>

using Color = uint32_t;

Color packColorRgb(uint32_t r, uint32_t g, uint32_t b);

const auto BLACK = packColorRgb(0, 0, 0);
const auto BULLET_PLAYER = packColorRgb(32, 32, 32);
const auto BULLET_ENEMY = packColorRgb(0, 32, 0);
const auto WHITE = packColorRgb(255, 255, 255);
const auto GRAY = packColorRgb(64, 64, 64);
const auto PINK = packColorRgb(255, 129, 185);
const auto RED = packColorRgb(255, 0, 0);
const auto BLUE = packColorRgb(0, 0, 255);
const auto BLOOD = packColorRgb(128, 0, 0);
const auto GREEN = packColorRgb(0, 255, 0);
const auto GOLD = packColorRgb(246, 183, 71);
const auto LAMP = packColorRgb(140, 220, 255);

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
    packColorRgb(0, 0, 0)
};
