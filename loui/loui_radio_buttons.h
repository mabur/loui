#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LouiRadioButtons {
    int x;
    int y;
    int width;
    int height;
    int selected_index;
    int count;
    const char** labels;
} LouiRadioButtons;

LouiRadioButtons loui_update_radio_buttons(LouiRadioButtons widget);

#ifdef __cplusplus
}
#endif
