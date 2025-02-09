#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    
typedef uint32_t GUI90_Color;

#define GUI90_RGB(r, g, b) ((GUI90_Color)((255 << 24) | (r << 16) | (g << 8) | (b << 0)))

GUI90_Color GUI90_InterpolateColors(GUI90_Color color0, GUI90_Color color1, uint32_t t);

    
#ifdef __cplusplus
}
#endif
