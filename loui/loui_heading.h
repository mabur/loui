#pragma once

#include "theme.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LouiHeading {
    int x;
    int y;
    int width;
    int height;
    const char* text;
    bool is_clicked;
    LouiHeadingTheme theme;
} LouiHeading;

LouiHeading loui_update_heading(LouiHeading widget);

#ifdef __cplusplus
}
#endif
