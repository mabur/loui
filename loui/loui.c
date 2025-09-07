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

LouiSlider loui_update_slider(LouiSlider widget) {
    auto horizontal_padding = LOUI_BLOCK;
    widget.height = LOUI_BLOCK * 2;
    auto x = widget.x;
    auto y = widget.y;
    auto width = widget.width;
    auto height = widget.height;
    auto rectangle = (Rectangle){.x=x, .y=y, .width=width, .height=height};

    LouiSunkenFrame sunken_frame = {.x=x+horizontal_padding, .y=y+7, .width=width - 2*horizontal_padding, .height=3};
    loui_update_sunken_frame(sunken_frame);

    auto background_color = getTheme().button_background;
    auto border_color = getTheme().button_border;
    auto light_bevel_color = getTheme().button_bevel_light;
    auto dark_bevel_color = getTheme().button_bevel_dark;

    if (isLeftMouseButtonDownInside(rectangle)) {
        light_bevel_color = background_color;
        dark_bevel_color = background_color;
        widget.value = (double)(s_loui.mouse_x - x - horizontal_padding) / (width - 1 - 2 * horizontal_padding);
        widget.value = widget.value < 0.0 ? 0.0 : widget.value;
        widget.value = widget.value > 1.0 ? 1.0 : widget.value;
    }

    auto button = (Rectangle){};
    button.width = 6;
    button.height = 13;
    button.x = sunken_frame.x + widget.value * (sunken_frame.width - button.width);
    button.y = widget.y + 2;

    // Begin draw button
    auto inner_rectangle = shrinkRectangle(button);
    auto innermost_rectangle = shrinkRectangle(inner_rectangle);
    auto screen = s_loui.screen;

    drawRoundedRectangleOutline(screen, button, border_color, border_color);
    drawRectangle(screen, innermost_rectangle, background_color);
    drawLineHorizontal(screen, inner_rectangle.x, inner_rectangle.y, inner_rectangle.width, light_bevel_color);
    drawLineVertical(screen, inner_rectangle.x, inner_rectangle.y, inner_rectangle.height, light_bevel_color);
    drawLineHorizontal(screen, inner_rectangle.x, inner_rectangle.y + inner_rectangle.height - 1, inner_rectangle.width, dark_bevel_color);
    drawLineVertical(screen, inner_rectangle.x + inner_rectangle.width - 1, inner_rectangle.y, inner_rectangle.height, dark_bevel_color);
    // End draw button

    widget.is_down = isLeftMouseButtonDownInside(rectangle);
    return widget;
}

LouiSelectionBoxInit loui_update_selection_box_init(LouiSelectionBoxInit widget) {
    s_loui.current_x = widget.x + TEXT_SIZE;
    s_loui.current_y = widget.y + TEXT_SIZE;
    auto frame = (LouiSunkenFrame){.x=widget.x, .y=widget.y, .width=widget.width, .height=widget.height};
    frame = loui_update_sunken_frame(frame);
    return widget;
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
