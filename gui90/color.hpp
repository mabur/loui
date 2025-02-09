#pragma once

#include <stdint.h>

typedef uint32_t GUI90_Color;

GUI90_Color GUI90_Rgb(uint32_t r, uint32_t g, uint32_t b);
GUI90_Color GUI90_InterpolateColors(GUI90_Color color0, GUI90_Color color1, uint32_t t);
