#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LouiSunkenFrame {
    int x;
    int y;
    int width;
    int height;
    bool is_clicked;
} LouiSunkenFrame;

LouiSunkenFrame loui_update_sunken_frame(LouiSunkenFrame widget);

#ifdef __cplusplus
}
#endif
