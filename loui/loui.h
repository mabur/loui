#pragma once

#include <stdbool.h>

#include "caret.h"
#include "color.h"
#include "state.h"
#include "theme.h"

#include "loui_button.h"
#include "loui_check_box.h"
#include "loui_heading.h"
#include "loui_knob.h"
#include "loui_label.h"
#include "loui_radio_button.h"
#include "loui_sunken_frame.h"
#include "loui_window.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LouiInput {
    int mouse_x;
    int mouse_y;
    int mouse_wheel_y;
    bool is_left_mouse_button_down;
    bool is_keyboard_key_down[LOUI_KEY_COUNT];
    bool is_modifier_key_down[LOUI_MODIFIER_KEY_COUNT];
    char input_character;
} LouiInput;

typedef struct LouiTextInput {
    int x;
    int y;
    int width;
    int height;
    char text[LOUI_MAX_SINGLE_LINE_TEXT_INPUT];
    SingleLineCaret caret;
    SingleLineCaret selection_anchor;
    bool is_clicked;
} LouiTextInput;

typedef struct LouiMultiTextInput {
    int x;
    int y;
    int width;
    int height;
    char text[LOUI_MAX_MULTI_LINE_TEXT_INPUT];
    int lines;
    int columns;
    MultiLineCaret caret;
    MultiLineCaret draw_caret;
    MultiLineCaret selection_anchor;
    bool is_clicked;
} LouiMultiTextInput;

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

typedef struct LouiSlider {
    int x;
    int y;
    int width;
    int height;
    double value;
    bool is_down;
} LouiSlider;

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

// Call once:
void loui_init(int width, int height);

// Call once per frame:
void loui_set_input(LouiInput input);
LouiColor* loui_get_pixel_data();

// Optionally call when needed:
void loui_enable_input();
void loui_disable_input();
void loui_set_theme(LouiTheme theme);

// Implementation details:
void loui_widget_background();

LouiStepper loui_update_stepper(LouiStepper widget);
LouiSlider loui_update_slider(LouiSlider widget);

LouiSelectionBoxInit loui_update_selection_box_init(LouiSelectionBoxInit widget);
LouiSelectionBoxItem loui_update_selection_box_item(LouiSelectionBoxItem widget);
LouiTextInput loui_update_text_input(LouiTextInput widget);
LouiMultiTextInput loui_update_multi_text_input(LouiMultiTextInput widget);

#ifdef __cplusplus
}
#endif

// Overloaded/generic loui_update function for widgets:

#ifdef __cplusplus
inline void loui_update(LouiLabel& widget) { widget = loui_update_label(widget);}
inline void loui_update(LouiHeading& widget) { widget = loui_update_heading(
        widget);}
inline void loui_update(LouiButton& widget) { widget = loui_update_button(widget);}
inline void loui_update(LouiKnob & widget) { widget = loui_update_knob(widget);}
inline void loui_update(LouiRadioButton& widget) { widget = loui_update_radio_button(widget);}
inline void loui_update(LouiCheckBox& widget) { widget = loui_update_check_box(widget);}
inline void loui_update(LouiStepper& widget) { widget = loui_update_stepper(widget);}
inline void loui_update(LouiSlider& widget) { widget = loui_update_slider(widget);}
inline void loui_update(LouiSelectionBoxInit& widget) { widget = loui_update_selection_box_init(widget);}
inline void loui_update(LouiSelectionBoxItem& widget) { widget = loui_update_selection_box_item(widget);}
inline void loui_update(LouiTextInput& widget) { widget = loui_update_text_input(widget);}
inline void loui_update(LouiMultiTextInput& widget) { widget = loui_update_multi_text_input(widget);}
inline void loui_update(LouiWindow& widget) { widget = loui_update_window(widget);}
#else
#define loui_update(widget) \
    do { widget = _Generic((widget), \
        LouiLabel: loui_update_label, \
        LouiHeading: loui_update_header_label, \
        LouiButton: loui_update_button, \
        LouiRadioButton: loui_update_radio_button, \
        LouiCheckBox: loui_update_check_box, \
        LouiStepper: loui_update_stepper, \
        LouiSlider: loui_update_slider, \
        LouiSelectionBoxInit: loui_update_selection_box_init, \
        LouiSelectionBoxItem: loui_update_selection_box_item, \
        LouiTextInput: loui_update_text_input, \
        LouiMultiTextInput: loui_update_multi_text_input, \
        LouiWindow: loui_update_window \
    )(widget); } while(0)
#endif
