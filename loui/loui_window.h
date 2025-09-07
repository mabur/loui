#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LouiWindow {
    int x;
    int y;
    int width;
    int height;
    bool is_clicked;
} LouiWindow;

LouiWindow loui_update_window(LouiWindow widget);

#ifdef __cplusplus
}
#endif
