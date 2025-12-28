#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "color.h"

typedef struct LouiImage {
    int x;
    int y;
    int width;
    int height;
    LouiColor* data;
    bool is_clicked;
} LouiImage;

LouiImage loui_update_image(LouiImage widget);

#ifdef __cplusplus
}
#endif
