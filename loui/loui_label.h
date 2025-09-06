#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LouiLabel {
    int x;
    int y;
    int width;
    int height;
    const char* text;
    bool is_clicked;
} LouiLabel;

LouiLabel loui_update_label(LouiLabel widget);

#ifdef __cplusplus
}
#endif
