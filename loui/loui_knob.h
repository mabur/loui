#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LouiKnob {
    int x;
    int y;
    int width;
    int height;
    double angle;
    bool is_clicked;
} LouiKnob;

LouiKnob loui_update_knob(LouiKnob widget);

#ifdef __cplusplus
}
#endif
