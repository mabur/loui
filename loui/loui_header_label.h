#pragma once

#include "theme.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LouiHeaderLabel {
    int x;
    int y;
    int width;
    int height;
    const char* text;
    bool is_clicked;
    LouiHeaderLabelTheme theme;
} LouiHeaderLabel;

LouiHeaderLabel loui_update_header_label(LouiHeaderLabel widget);

#ifdef __cplusplus
}
#endif
