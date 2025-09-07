#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LouiStepper {
    int x;
    int y;
    int width;
    int height;
    const char* text;
    bool is_clicked;
    bool is_increased;
    bool is_decreased;
} LouiStepper;

LouiStepper loui_update_stepper(LouiStepper widget);

#ifdef __cplusplus
}
#endif
