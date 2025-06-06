#pragma once

#include <stdbool.h>

#include "caret.h"
#include "color.h"
#include "theme.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LOUI_BLOCK 8
#define LOUI_MAX_SINGLE_LINE_TEXT_INPUT 16
#define LOUI_MAX_MULTI_LINE_TEXT_INPUT 1024

typedef enum LouiKeyboard {
    LOUI_KEY_ARROW_DOWN,
    LOUI_KEY_ARROW_LEFT,
    LOUI_KEY_ARROW_RIGHT,
    LOUI_KEY_ARROW_UP,
    LOUI_KEY_BACKSPACE,
    LOUI_KEY_DELETE,
    LOUI_KEY_END,
    LOUI_KEY_ENTER,
    LOUI_KEY_HOME,
    LOUI_KEY_PAGE_DOWN,
    LOUI_KEY_PAGE_UP,
    LOUI_KEY_COUNT,
} LouiKeyboard;

typedef struct LouiInput {
    int mouse_x;
    int mouse_y;
    int mouse_wheel_y;
    bool is_left_mouse_button_down;
    bool is_keyboard_key_down[LOUI_KEY_COUNT];
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
    LouiHeaderLabelTheme theme;
} LouiHeaderLabel;

typedef struct LouiSunkenFrame {
    int x;
    int y;
    int width;
    int height;
    bool is_clicked;
} LouiSunkenFrame;

typedef struct LouiWindow {
    int x;
    int y;
    int width;
    int height;
    bool is_clicked;
} LouiWindow;

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

typedef struct LouiCheckBox {
    int x;
    int y;
    int width;
    int height;
    const char* text;
    bool is_selected;
    bool is_clicked;
} LouiCheckBox;

typedef struct LouiTextInput {
    int x;
    int y;
    int width;
    int height;
    char text[LOUI_MAX_SINGLE_LINE_TEXT_INPUT];
    SingleLineCaret caret;
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
LouiLabel loui_update_label(LouiLabel widget);
LouiHeaderLabel loui_update_header_label(LouiHeaderLabel widget);
LouiButton loui_update_button(LouiButton widget);
LouiRadioButton loui_update_radio_button(LouiRadioButton widget);
LouiCheckBox loui_update_check_box(LouiCheckBox widget);
LouiStepper loui_update_stepper(LouiStepper widget);
LouiSunkenFrame loui_update_sunken_frame(LouiSunkenFrame widget);
LouiWindow loui_update_window(LouiWindow widget);
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
inline void loui_update(LouiHeaderLabel& widget) { widget = loui_update_header_label(widget);}
inline void loui_update(LouiButton& widget) { widget = loui_update_button(widget);}
inline void loui_update(LouiRadioButton& widget) { widget = loui_update_radio_button(widget);}
inline void loui_update(LouiCheckBox& widget) { widget = loui_update_check_box(widget);}
inline void loui_update(LouiStepper& widget) { widget = loui_update_stepper(widget);}
inline void loui_update(LouiSelectionBoxInit& widget) { widget = loui_update_selection_box_init(widget);}
inline void loui_update(LouiSelectionBoxItem& widget) { widget = loui_update_selection_box_item(widget);}
inline void loui_update(LouiTextInput& widget) { widget = loui_update_text_input(widget);}
inline void loui_update(LouiMultiTextInput& widget) { widget = loui_update_multi_text_input(widget);}
inline void loui_update(LouiWindow& widget) { widget = loui_update_window(widget);}
#else
#define loui_update(widget) \
    do { widget = _Generic((widget), \
        LouiLabel: loui_update_label, \
        LouiHeaderLabel: loui_update_header_label, \
        LouiButton: loui_update_button, \
        LouiRadioButton: loui_update_radio_button, \
        LouiCheckBox: loui_update_check_box, \
        LouiStepper: loui_update_stepper, \
        LouiSelectionBoxInit: loui_update_selection_box_init, \
        LouiSelectionBoxItem: loui_update_selection_box_item, \
        LouiTextInput: loui_update_text_input, \
        LouiMultiTextInput: loui_update_multi_text_input, \
        LouiWindow: loui_update_window \
    )(widget); } while(0)
#endif
