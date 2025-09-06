#pragma once

#include "button.h"
#include "draw.h"
#include "theme.h"

typedef enum LouiKey {
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
} LouiKey;

typedef enum LouiModifierKey {
    LOUI_MODIFIER_KEY_SHIFT,
    LOUI_MODIFIER_KEY_COUNT,
} LouiModifierKey;

typedef struct LouiState {
    LouiScreen screen;
    int mouse_x;
    int mouse_y;
    int mouse_wheel_y;
    int current_x;
    int current_y;
    LouiTheme theme;
    ButtonState left_mouse_button;
    RepeatingButtonState keyboard_keys[LOUI_KEY_COUNT];
    ButtonState modifier_keys[LOUI_MODIFIER_KEY_COUNT];
    char input_character;
    int active_text_input_widget_index;
    int text_input_widget_index_count;
    bool is_input_enabled;
} LouiState;

extern LouiState s_loui;

LouiTheme getTheme();
