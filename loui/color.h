#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    
typedef uint32_t LouiColor;

#define LOUI_RGB(r, g, b) ((LouiColor)((255 << 24) | (r << 16) | (g << 8) | (b << 0)))

LouiColor loui_interpolate_colors(LouiColor color0, LouiColor color1, uint32_t t);

    
#ifdef __cplusplus
}
#endif
