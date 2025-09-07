#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LouiCheckBox {
    int x;
    int y;
    int width;
    int height;
    const char* text;
    bool is_selected;
    bool is_clicked;
} LouiCheckBox;

LouiCheckBox loui_update_check_box(LouiCheckBox widget);

#ifdef __cplusplus
}
#endif
