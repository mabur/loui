#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "color.h"

typedef struct ImageBuffer {
    LouiColor* data;
    int width;
    int height;
    int count;
} ImageBuffer;

typedef struct LouiImage {
    int x;
    int y;
    ImageBuffer image;
    bool is_clicked;
} LouiImage;

LouiImage loui_update_image(LouiImage widget);

ImageBuffer parse_ppm(const char* ppm_string);

#ifdef __cplusplus
}
#endif
