#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LouiRadioButton {
    int x;
    int y;
    int width;
    int height;
    const char* text;
    bool is_selected;
    bool is_clicked;
} LouiRadioButton;

LouiRadioButton loui_update_radio_button(LouiRadioButton widget);

#ifdef __cplusplus
}
#endif
