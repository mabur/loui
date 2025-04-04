#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "color.h"
#include "theme.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GUI90_BLOCK 8
#define GUI90_MAX_SINGLE_LINE_TEXT_INPUT 16

typedef struct LouiInput {
    int mouse_x;
    int mouse_y;
    bool is_left_mouse_button_down;
    bool is_left_arrow_button_down;
    bool is_right_arrow_button_down;
    bool is_backspace_button_down;
    bool is_delete_button_down;
    bool is_home_button_down;
    bool is_end_button_down;
    char input_character;
} LouiInput;

typedef struct LouiLabel {
    int x;
    int y;
    int width;
    int height;
    const char* text;
    bool is_clicked;
} LouiLabel;

typedef struct LouiHeaderLabel {
    int x;
    int y;
    int width;
    int height;
    const char* text;
    bool is_clicked;
    GUI90_HeaderLabelTheme theme;
} LouiHeaderLabel;

typedef struct LouiSunkenFrame {
    int x;
    int y;
    int width;
    int height;
    bool is_clicked;
} LouiSunkenFrame;

typedef struct LouiButton {
    int x;
    int y;
    int width;
    int height;
    const char* text;
    bool is_clicked;
} LouiButton;

typedef struct LouiRadioButton {
    int x;
    int y;
    int width;
    int height;
    const char* text;
    bool is_selected;
    bool is_clicked;
} LouiRadioButton;

typedef struct LouiTextInput {
    int x;
    int y;
    int width;
    int height;
    char text[GUI90_MAX_SINGLE_LINE_TEXT_INPUT];
    int cursor;
    bool is_clicked;
} LouiTextInput;

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

typedef struct LouiSelectionBoxInit {
    int x;
    int y;
    int width;
    int height;
    bool is_clicked;
} LouiSelectionBoxInit;

typedef struct LouiSelectionBoxItem {
    int x;
    int y;
    int width;
    int height;
    const char* text;
    bool is_selected;
    bool is_clicked;
} LouiSelectionBoxItem;

void loui_init(int width, int height);
void loui_set_input(LouiInput input);
void loui_set_theme(GUI90_Theme theme);
const GUI90_Color* loui_get_pixel_data();

void loui_widget_background();

LouiLabel loui_update_label(LouiLabel widget);
LouiHeaderLabel loui_update_header_label(LouiHeaderLabel widget);
LouiButton loui_update_button(LouiButton widget);
LouiRadioButton loui_update_radio_button(LouiRadioButton widget);
LouiStepper loui_update_stepper(LouiStepper widget);
LouiSunkenFrame loui_update_sunken_frame(LouiSunkenFrame widget);
LouiSelectionBoxInit loui_update_selection_box_init(LouiSelectionBoxInit widget);
LouiSelectionBoxItem loui_update_selection_box_item(LouiSelectionBoxItem widget);
LouiTextInput loui_update_text_input(LouiTextInput widget);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
inline void loui_update(LouiLabel& widget) { widget = loui_update_label(widget);}
inline void loui_update(LouiHeaderLabel& widget) { widget = loui_update_header_label(widget);}
inline void loui_update(LouiButton& widget) { widget = loui_update_button(widget);}
inline void loui_update(LouiRadioButton& widget) { widget = loui_update_radio_button(widget);}
inline void loui_update(LouiStepper& widget) { widget = loui_update_stepper(widget);}
inline void loui_update(LouiSelectionBoxInit& widget) { widget = loui_update_selection_box_init(widget);}
inline void loui_update(LouiSelectionBoxItem& widget) { widget = loui_update_selection_box_item(widget);}
inline void loui_update(LouiTextInput& widget) { widget = loui_update_text_input(widget);}
#else
#define GUI90_Update(widget) \
    do { widget = _Generic((widget), \
        LouiLabel: GUI90_UpdateLabel, \
        LouiHeaderLabel: GUI90_UpdateHeaderLabel, \
        LouiButton: GUI90_UpdateButton, \
        LouiRadioButton: GUI90_UpdateRadioButton, \
        LouiStepper: GUI90_UpdateStepper, \
        LouiSelectionBoxInit: GUI90_UpdateSelectionBoxInit, \
        LouiSelectionBoxItem: GUI90_UpdateSelectionBoxItem, \
        LouiTextInput: GUI90_UpdateTextInput \
    )(widget); } while(0)
#endif
