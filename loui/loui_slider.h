#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LouiSlider {
    int x;
    int y;
    int width;
    int height;
    double value;
    bool is_down;
} LouiSlider;

LouiSlider loui_update_slider(LouiSlider widget);

#ifdef __cplusplus
}
#endif
