#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Rectangle Rectangle;

typedef struct LouiButton {
    int x;
    int y;
    int width;
    int height;
    const char* text;
    bool is_clicked;
} LouiButton;

LouiButton loui_update_button(LouiButton widget);
void drawButton(Rectangle rectangle, const char* text);

#ifdef __cplusplus
}
#endif
