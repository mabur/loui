#include "loui.h"

#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "button.h"
#include "draw.h"
#include "rectangle.h"

#define DRAW_DEBUG_RECTANGLES 0

// -----------------------------------------------------------------------------
// PUBLIC FUNCTIONS

static LouiScreen initScreen(int width, int height) {
    auto data = (LouiColor*)malloc(width * height * sizeof(LouiColor));
    if (data) {
        return (LouiScreen){.data=data, .width=width, .height=height, .count=width * height};
    }
    return (LouiScreen){};
}

static LouiScreen freeScreen(LouiScreen screen) {
    free(screen.data);
    return (LouiScreen){};
}

void loui_init(int width, int height) {
    freeScreen(s_loui.screen);
    s_loui = (LouiState){};
    s_loui.screen = initScreen(width, height);
    s_loui.theme = LOUI_THEME_GRAY;
    s_loui.active_text_input_widget_index = -1;
    s_loui.is_input_enabled = true;
}

void loui_set_input(LouiInput input) {
    // Mouse:
    s_loui.mouse_x = input.mouse_x;
    s_loui.mouse_y = input.mouse_y;
    s_loui.mouse_wheel_y = input.mouse_wheel_y;
    s_loui.left_mouse_button = updateButtonState(s_loui.left_mouse_button, input.is_left_mouse_button_down);
    // Keyboard keys:
    for (auto i = 0; i < LOUI_KEY_COUNT; ++i) {
        s_loui.keyboard_keys[i] = updateRepeatingButtonState(
            s_loui.keyboard_keys[i], input.is_keyboard_key_down[i]
        );
    }
    // Modifier keys:
    for (auto i = 0; i < LOUI_MODIFIER_KEY_COUNT; ++i) {
        s_loui.modifier_keys[i] = updateButtonState(
            s_loui.modifier_keys[i], input.is_modifier_key_down[i]
        );
    }
    s_loui.input_character = input.input_character;
    // Other:
    s_loui.text_input_widget_index_count = 0;
}

void loui_enable_input() {
    s_loui.is_input_enabled = true;
}

void loui_disable_input() {
    s_loui.is_input_enabled = false;
}

LouiColor* loui_get_pixel_data() {
    return s_loui.screen.data;
}

void loui_widget_background() {
    auto pixel_count = s_loui.screen.width * s_loui.screen.height;
    for (int i = 0; i < pixel_count; ++i) {
        s_loui.screen.data[i] = getTheme().background;
    }

    if (DRAW_DEBUG_RECTANGLES) {
        auto i = 0;
        for (auto y = 0; y < s_loui.screen.height; ++y) {
            for (auto x = 0; x < s_loui.screen.width; ++x, ++i) {
                if ((x / 8) % 2 != (y / 8 + 1) % 2) {
                    s_loui.screen.data[i] = LOUI_RGB(0, 255, 0);
                }
            }
        }
    }
}

LouiSelectionBoxItem loui_update_selection_box_item(LouiSelectionBoxItem widget) {
    auto global_theme = getTheme();
    auto local_theme = getTheme();
    local_theme.text = widget.is_selected ? local_theme.recess_text_selected : local_theme.recess_text;
    loui_set_theme(local_theme);
    auto label = (LouiLabel){
        .x=s_loui.current_x,
        .y=s_loui.current_y,
        .text=widget.text
    };
    label = loui_update_label(label);
    s_loui.current_y += label.height;
    loui_set_theme(global_theme);
    widget.width = label.width;
    widget.height = label.height;
    widget.is_clicked = label.is_clicked;
    return widget;
}
