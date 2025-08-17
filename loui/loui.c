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

// -----------------------------------------------------------------------------
// PRIVATE STUFF

static const int TEXT_SIZE = 8;
static const int BUTTON_TEXT_PADDING = 4;

static LouiState s_loui;

// -----------------------------------------------------------------------------
// PRIVATE MOUSE & KEYBOARD FUNCTIONS

static bool isLeftMouseButtonDownInside(Rectangle r) {
    return s_loui.is_input_enabled && s_loui.left_mouse_button == BUTTON_DOWN &&
           r.x <= s_loui.mouse_x && s_loui.mouse_x < r.x + r.width &&
           r.y <= s_loui.mouse_y && s_loui.mouse_y < r.y + r.height;
}

static bool isLeftMouseButtonReleasedInside(Rectangle r) {
    return s_loui.is_input_enabled && s_loui.left_mouse_button == BUTTON_RELEASED &&
           r.x <= s_loui.mouse_x && s_loui.mouse_x < r.x + r.width &&
           r.y <= s_loui.mouse_y && s_loui.mouse_y < r.y + r.height;
}

// -----------------------------------------------------------------------------
// PRIVATE DRAW FUNCTIONS

static void drawCaret(
    LouiScreen screen, size_t x_start, size_t y_start, LouiColor color) {
    for (size_t y = 0; y < 8; ++y) {
        screen.data[(y_start + y) * screen.width + x_start] = color;
    }
}

static void drawMultiLineCaret(
    LouiScreen screen,
    size_t x_start,
    size_t y_start,
    LouiColor color,
    MultiLineCaret caret,
    MultiLineCaret draw_caret
) {
    auto draw_caret_line = caret.line - draw_caret.line;
    auto draw_caret_column = caret.column - draw_caret.column;
    auto caret_x = x_start + draw_caret_column * TEXT_SIZE;
    auto caret_y = y_start + draw_caret_line * TEXT_SIZE;
    drawCaret(screen, caret_x, caret_y, color);
}

static void drawMultiLineString(
    LouiScreen screen,
    const char* s,
    size_t x,
    size_t y,
    LouiColor color,
    int max_lines,
    int max_columns,
    MultiLineCaret draw_caret
) {
    auto line = 0;
    auto column = 0;
    for (; *s; ++s) {
        auto draw_line = line - draw_caret.line;
        auto draw_column = column - draw_caret.column;
        auto draw_x = x + draw_column * TEXT_SIZE;
        auto draw_y = y + draw_line * TEXT_SIZE;
        if (0 <= draw_line && draw_line < max_lines &&
            0 <= draw_column && draw_column < max_columns
        ) {
            drawCharacter(screen, *s, draw_x, draw_y, color);
        }
        if (*s == '\n') {
            column = 0;
            line++;
        }
        else {
            column++;
        }
    }
}

static void drawSpecialString(
    LouiScreen screen, const char* s, int x, int y, LouiHeaderLabelTheme theme
) {
    for (; *s; ++s, x += 8) {
        if (theme.draw_up_left) {
            drawCharacter(screen, *s, x - 1, y - 1, theme.color_up_left);
        }
        if (theme.draw_up_right) {
            drawCharacter(screen, *s, x + 1, y - 1, theme.color_up_right);
        }
        if (theme.draw_down_left) {
            drawCharacter(screen, *s, x - 1, y + 1, theme.color_down_left);
        }
        if (theme.draw_down_right) {
            drawCharacter(screen, *s, x + 1, y + 1, theme.color_down_right);
        }

        if (theme.draw_up) {
            drawCharacter(screen, *s, x + 0, y - 1, theme.color_up);
        }
        if (theme.draw_left) {
            drawCharacter(screen, *s, x - 1, y + 0, theme.color_left);
        }
        if (theme.draw_right) {
            drawCharacter(screen, *s, x + 1, y + 0, theme.color_right);
        }
        if (theme.draw_down) {
            drawCharacter(screen, *s, x + 0, y + 1, theme.color_down);
        }

        if (theme.draw_center) {
            drawCharacter(screen, *s, x + 0, y + 0, theme.color_center);
        }
    }
}

static void drawButton(Rectangle rectangle, const char* text) {
    // Inside the bevels:
    auto inner_rectangle = shrinkRectangle(rectangle);
    auto innermost_rectangle = shrinkRectangle(inner_rectangle);
    auto text_x = rectangle.x - 1 + BUTTON_TEXT_PADDING;
    auto text_y = rectangle.y - 1 + BUTTON_TEXT_PADDING;
    auto theme = s_loui.theme;
    auto screen = s_loui.screen;

    auto background_color = s_loui.theme.button_background;
    auto border_color = s_loui.theme.button_border;
    auto light_bevel_color = s_loui.theme.button_bevel_light;
    auto dark_bevel_color = s_loui.theme.button_bevel_dark;

    if (isLeftMouseButtonDownInside(rectangle)) {
        ++text_y;
        light_bevel_color = background_color;
        dark_bevel_color = background_color;
    }
    drawRectangle(screen, rectangle, border_color);
    drawRectangle(screen, innermost_rectangle, background_color);
    drawRoundedRectangleOutline(
        screen, inner_rectangle, light_bevel_color, dark_bevel_color
    );
    drawString(screen, text, text_x, text_y, theme.button_text);
}

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

void loui_set_theme(LouiTheme theme) {
    s_loui.theme = theme;
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
        s_loui.screen.data[i] = s_loui.theme.background;
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

static Rectangle textRectangle(int x, int y, const char* text) {
    return (Rectangle){
        .x = x,
        .y = y,
        .width = 8 * (int)(strlen(text)),
        .height = 8,    
    };
}

LouiSunkenFrame loui_update_sunken_frame(LouiSunkenFrame widget) {
    auto x = widget.x;
    auto y = widget.y;
    auto width = widget.width;
    auto height = widget.height;
    auto rectangle = (Rectangle){x, y, width, height};
    auto inner_rectangle = shrinkRectangle(rectangle);
    drawRectangle(s_loui.screen, inner_rectangle, s_loui.theme.recess_background);
    drawRoundedRectangleOutline(
        s_loui.screen, rectangle, s_loui.theme.recess_bevel_dark, s_loui.theme.recess_bevel_light
    );
    widget.is_clicked = isLeftMouseButtonReleasedInside(inner_rectangle);
    return widget;
}

LouiWindow loui_update_window(LouiWindow widget) {
    auto x = widget.x;
    auto y = widget.y;
    auto width = widget.width;
    auto height = widget.height;
    auto rectangle = (Rectangle){x, y, width, height};
    auto inner_rectangle = shrinkRectangle(rectangle);
    auto innermost_rectangle = shrinkRectangle(inner_rectangle);

    auto border_color = s_loui.theme.button_border;
    auto background_color = s_loui.theme.background;
    auto light_bevel_color = s_loui.theme.recess_bevel_light;
    auto dark_bevel_color = s_loui.theme.recess_bevel_dark;

    drawRoundedRectangleOutline(s_loui.screen, rectangle, border_color, border_color);
    drawRoundedRectangleOutline(s_loui.screen, inner_rectangle, light_bevel_color, dark_bevel_color);
    drawRectangleCorners(s_loui.screen, inner_rectangle, border_color);
    drawRectangle(s_loui.screen, innermost_rectangle, background_color);

    widget.is_clicked = isLeftMouseButtonReleasedInside(innermost_rectangle);
    return widget;
}

LouiLabel loui_update_label(LouiLabel widget) {
    drawString(s_loui.screen, widget.text, widget.x, widget.y, s_loui.theme.text);
    auto rectangle = textRectangle(widget.x, widget.y, widget.text);
    widget.width = rectangle.width;
    widget.height = rectangle.height;
    widget.is_clicked = isLeftMouseButtonReleasedInside(rectangle);
    return widget;
}

LouiHeaderLabel loui_update_header_label(LouiHeaderLabel widget) {
    drawSpecialString(s_loui.screen, widget.text, widget.x, widget.y, widget.theme);
    auto rectangle = textRectangle(widget.x, widget.y, widget.text);
    widget.width = rectangle.width;
    widget.height = rectangle.height;
    widget.is_clicked = isLeftMouseButtonReleasedInside(rectangle);
    return widget;
}

LouiButton loui_update_button_bevel(LouiButton widget) {
    auto rectangle = (Rectangle){};
    rectangle.x = widget.x + 1;
    rectangle.y = widget.y + 1;
    rectangle.width = 8 * (int)(strlen(widget.text)) + 6;
    rectangle.height = 8 + 6;

    drawButton(rectangle, widget.text);

    widget.width = rectangle.width + 2;
    widget.height = rectangle.height + 2;
    widget.is_clicked = isLeftMouseButtonReleasedInside(rectangle);
    return widget;
}

LouiButton loui_update_button_cloud(LouiButton widget) {
    auto x = widget.x;
    auto y = widget.y;
    x += 1;
    y += 1;
    auto rectangle = (Rectangle){};
    rectangle.x = x;
    rectangle.y = y;
    rectangle.width = 8 * (int)(strlen(widget.text)) + 6;
    rectangle.height = 8 + 6 - 1;

    if (isLeftMouseButtonDownInside(rectangle)) {
        rectangle.y += 1;
    }
    
    auto inner_rectangle = shrinkRectangle(rectangle);
    auto text_x = x + BUTTON_TEXT_PADDING - 1;
    auto text_y = y + BUTTON_TEXT_PADDING - 2;
    auto theme = s_loui.theme;
    if (isLeftMouseButtonDownInside(rectangle)) {
        ++text_y;
    }

    // Shadow:
    drawPoint(s_loui.screen, x, y + rectangle.height - 1, theme.button_border);
    drawPoint(s_loui.screen, x + rectangle.width - 1, y + rectangle.height - 1, theme.button_border);
    drawLineHorizontal(s_loui.screen, x + 1, y + rectangle.height, rectangle.width - 2, theme.button_border);
    
    //drawRectangle(rectangle, s_loui.theme.button_border);
    drawRectangle(s_loui.screen, inner_rectangle, theme.button_background);
    
    // Rounded corners:
    drawLineHorizontal(s_loui.screen, rectangle.x + 1, rectangle.y, rectangle.width - 2, theme.button_border);
    drawLineHorizontal(s_loui.screen, rectangle.x + 1, rectangle.y + rectangle.height - 1, rectangle.width - 2, theme.button_border);
    drawLineVertical(s_loui.screen, rectangle.x, rectangle.y + 1, rectangle.height - 2, theme.button_border);
    drawLineVertical(s_loui.screen, rectangle.x + rectangle.width - 1, rectangle.y + 1, rectangle.height - 2, theme.button_border);
    drawPoint(s_loui.screen, rectangle.x + 1, rectangle.y + 1, theme.button_border);
    drawPoint(s_loui.screen, rectangle.x + 1, rectangle.y + rectangle.height - 2, theme.button_border);
    drawPoint(s_loui.screen, rectangle.x + + rectangle.width - 2, rectangle.y + 1, theme.button_border);
    drawPoint(s_loui.screen, rectangle.x + + rectangle.width - 2, rectangle.y + rectangle.height - 2, theme.button_border);
    
    drawPoint(s_loui.screen, rectangle.x + 2, rectangle.y + 1, theme.button_bevel_dark);
    drawPoint(s_loui.screen, rectangle.x + 1, rectangle.y + 2, theme.button_bevel_dark);
    drawPoint(s_loui.screen, rectangle.x + 1, rectangle.y + rectangle.height - 3, theme.button_bevel_dark);
    drawPoint(s_loui.screen, rectangle.x + 2, rectangle.y + rectangle.height - 2, theme.button_bevel_dark);
    drawPoint(s_loui.screen, rectangle.x + rectangle.width - 3, rectangle.y + 1, theme.button_bevel_dark);
    drawPoint(s_loui.screen, rectangle.x + rectangle.width - 2, rectangle.y + 2, theme.button_bevel_dark);
    drawPoint(s_loui.screen, rectangle.x + rectangle.width - 3, rectangle.y + rectangle.height - 2, theme.button_bevel_dark);
    drawPoint(s_loui.screen, rectangle.x + rectangle.width - 2, rectangle.y + rectangle.height - 3, theme.button_bevel_dark);
    
    drawString(s_loui.screen, widget.text, text_x, text_y, theme.button_text);
    widget.width = rectangle.width + 2;
    widget.height = 2 * LOUI_BLOCK;
    widget.is_clicked = isLeftMouseButtonReleasedInside(rectangle);
    return widget;
}

LouiButton loui_update_button(LouiButton widget) {
    switch (s_loui.theme.button_type) {
        case BUTTON_TYPE_BEVEL: return loui_update_button_bevel(widget);
        case BUTTON_TYPE_CLOUD: return loui_update_button_cloud(widget);
        default: return loui_update_button_bevel(widget);
    }
}

LouiKnob loui_update_knob(LouiKnob widget) {
    widget.width = 16;
    widget.height = 16;
    auto rectangle = (Rectangle){widget.x, widget.y, widget.width, widget.height};
    widget.is_clicked = isLeftMouseButtonReleasedInside(rectangle);
    if (isLeftMouseButtonDownInside(rectangle)) {
        auto dx = s_loui.mouse_x - (widget.x + 7.5);
        auto dy = s_loui.mouse_y - (widget.y + 7.5);
        widget.angle = atan2(dy, dx);
    }
    for (int yi = 0; yi < 16; ++yi) {
        for (int xi = 0; xi < 16; ++xi) {
            double dx = xi - 7.5;
            double dy = yi - 7.5;
            double angle = atan2(dy, dx);
            double da = fabs(angle - widget.angle);
            double r2 = dx * dx + dy * dy;
            auto color = s_loui.theme.background;
            if (r2 < 6.0 * 6.0 && dx + dy < 0.0) {
                color = s_loui.theme.recess_bevel_dark;
            }
            if (r2 < 6.0 * 6.0 && dx + dy > 0.0) {
                color = s_loui.theme.recess_bevel_light;
            }
            if (r2 < 5.0 * 5.0) {
                color = s_loui.theme.recess_background;
            }
            if (r2 < 5.0 * 5.0 && da < 3.14 * 0.07) {
                color = s_loui.theme.recess_text_selected;
            }
            if (r2 < 2.0 * 2.0 && dx + dy < 0.0) {
                color = s_loui.theme.recess_bevel_light;
            }
            if (r2 < 2.0 * 2.0 && dx + dy > 0.0) {
                color = s_loui.theme.recess_bevel_dark;
            }
            if (r2 < 1.0 * 1.0) {
                color = s_loui.theme.background;
            }
            drawPoint(s_loui.screen, widget.x + xi, widget.y + yi, color);
        }
    }
    return widget;
}

LouiRadioButton loui_update_radio_button(LouiRadioButton widget) {
    for (int yi = 0; yi < 16; ++yi) {
        for (int xi = 0; xi < 16; ++xi) {
            double dx = xi - 7.5;
            double dy = yi - 7.5;
            double r2 = dx * dx + dy * dy;
            auto color = s_loui.theme.background;
            if (r2 < 6.0 * 6.0 && dx + dy < 0.0) {
                color = s_loui.theme.recess_bevel_dark;
            }
            if (r2 < 6.0 * 6.0 && dx + dy > 0.0) {
                color = s_loui.theme.recess_bevel_light;
            }
            if (r2 < 5.0 * 5.0) {
                color = s_loui.theme.recess_background;
            }
            if (r2 < 2.0 * 2.0 && widget.is_selected) {
                color = s_loui.theme.recess_text_selected;
            }
            drawPoint(s_loui.screen, widget.x + xi - 2, widget.y + yi, color);
        }
    }
    auto left_rectangle = (Rectangle){widget.x, widget.y, 16 + BUTTON_TEXT_PADDING, 16};
    auto label = (LouiLabel){
        .x=widget.x + 2 * LOUI_BLOCK,
        .y=widget.y + BUTTON_TEXT_PADDING,
        .text=widget.text
    };
    auto label_result = loui_update_label(label);
    widget.width = label_result.width + 3 * LOUI_BLOCK;
    widget.height = 2 * LOUI_BLOCK;
    widget.is_clicked = label_result.is_clicked || isLeftMouseButtonReleasedInside(left_rectangle);
    return widget;
}

LouiCheckBox loui_update_check_box(LouiCheckBox widget) {
    auto x = widget.x;
    auto y = widget.y;
    auto box_size = 10;
    auto frame = (LouiSunkenFrame){
        .x=x,// + (16 - box_size) / 2,
        .y=y + (16 - box_size) / 2,
        .width=box_size,
        .height=box_size,
    };
    frame = loui_update_sunken_frame(frame);

    int check[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 0, 0,
        0, 0, 1, 0, 0, 1, 0, 0,
        0, 0, 0, 1, 1, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
    };
    if (widget.is_selected) {
        for (auto dy = 0; dy < 8; ++dy) {
            for (auto dx = 0; dx < 8; ++dx) {
                if (check[dy * 8 + dx]) {
                    drawPoint(s_loui.screen, x + 1 + dx, y + 4 + dy, s_loui.theme.recess_text_selected);
                }
            }
        }
    }

    auto left_rectangle = (Rectangle){widget.x, widget.y, 16 + BUTTON_TEXT_PADDING, 16};
    auto label = (LouiLabel){
        .x=widget.x + 2 * LOUI_BLOCK,
        .y=widget.y + BUTTON_TEXT_PADDING,
        .text=widget.text
    };
    auto label_result = loui_update_label(label);
    widget.width = label_result.width + 3 * LOUI_BLOCK;
    widget.height = 2 * LOUI_BLOCK;
    widget.is_clicked = label_result.is_clicked || isLeftMouseButtonReleasedInside(left_rectangle);
    if (widget.is_clicked) {
        widget.is_selected = !widget.is_selected;
    }
    return widget;
}

LouiStepper loui_update_stepper(LouiStepper widget) {
    auto x = widget.x;
    auto y = widget.y;
    auto offset = 0;
    auto label = (LouiLabel){
        .x=x + offset,
        .y=y + BUTTON_TEXT_PADDING,
        .text=widget.text
    };
    auto label_widget = loui_update_label(label);
    offset += label_widget.width;
    auto decrease_button = (LouiButton){.x=x + offset, .y=y, .text="-"};
    decrease_button = loui_update_button(decrease_button);
    offset += decrease_button.width;
    auto increase_button = (LouiButton){.x=x + offset, .y=y, .text="+"};
    increase_button = loui_update_button(increase_button);
    offset += increase_button.width;

    widget.width = offset;
    widget.height = increase_button.height;
    widget.is_clicked = increase_button.is_clicked || decrease_button.is_clicked;
    widget.is_increased = increase_button.is_clicked;
    widget.is_decreased = decrease_button.is_clicked;
    return widget;
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

    auto background_color = s_loui.theme.button_background;
    auto border_color = s_loui.theme.button_border;
    auto light_bevel_color = s_loui.theme.button_bevel_light;
    auto dark_bevel_color = s_loui.theme.button_bevel_dark;

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
    auto global_theme = s_loui.theme;
    auto local_theme = s_loui.theme;
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

LouiTextInput loui_update_text_input(LouiTextInput widget) {
    auto widget_index = s_loui.text_input_widget_index_count++;
    auto is_selected = s_loui.active_text_input_widget_index == widget_index;
    auto keyboard = s_loui.keyboard_keys;
    if (is_selected) {
        if (s_loui.input_character) {
            widget.caret = insertCharacterSingleLineCaret(
                widget.caret,
                widget.text,
                LOUI_MAX_SINGLE_LINE_TEXT_INPUT,
                s_loui.input_character
            );
        }
        if (isClicked(keyboard[LOUI_KEY_HOME])) {
            widget.caret = moveSingleLineCaretHome(widget.caret, widget.text);
        }
        if (isClicked(keyboard[LOUI_KEY_END])) {
            widget.caret = moveSingleLineCaretEnd(widget.caret, widget.text);
        }
        if (isClicked(keyboard[LOUI_KEY_ARROW_LEFT])) {
            widget.caret = moveSingleLineCaretLeft(widget.caret, widget.text);
        }
        if (isClicked(keyboard[LOUI_KEY_ARROW_RIGHT])) {
            widget.caret = moveSingleLineCaretRight(widget.caret, widget.text);
        }
        if (isClicked(keyboard[LOUI_KEY_DELETE])) {
            widget.caret = deleteCharacterAfterSingleLineCaret(widget.caret, widget.text);
        }
        if (isClicked(keyboard[LOUI_KEY_BACKSPACE])) {
            widget.caret = deleteCharacterBeforeSingleLineCaret(widget.caret, widget.text);
        }
    }

    auto x = widget.x;
    auto y = widget.y;
    auto text_x = x + LOUI_BLOCK / 2;
    auto text_y = y + LOUI_BLOCK / 2;
    auto width = 8 * (LOUI_MAX_SINGLE_LINE_TEXT_INPUT - 1) + LOUI_BLOCK;
    auto height = 8 + LOUI_BLOCK;
    widget.width = width;
    widget.height = height;

    auto frame = (LouiSunkenFrame){.x=x, .y=y, .width=width, .height=height};
    frame = loui_update_sunken_frame(frame);

    widget.is_clicked = frame.is_clicked;
    if (widget.is_clicked) {
        s_loui.active_text_input_widget_index = widget_index;
        auto column = (s_loui.mouse_x - text_x + TEXT_SIZE / 4) / TEXT_SIZE;
        widget.caret = moveSingleLineCaretColumn(widget.caret, widget.text,
            column);
    }

    auto global_theme = s_loui.theme;
    auto local_theme = s_loui.theme;
    local_theme.text = is_selected ? local_theme.recess_text_selected : local_theme.recess_text;
    loui_set_theme(local_theme);
    drawString(s_loui.screen, widget.text, text_x, text_y, s_loui.theme.text);
    if (is_selected) {
        auto cursor_x = text_x + widget.caret.column * TEXT_SIZE;
        auto cursor_y = text_y;
        drawCaret(s_loui.screen, cursor_x, cursor_y, s_loui.theme.text);
    }
    loui_set_theme(global_theme);

    return widget;
}

LouiMultiTextInput loui_update_multi_text_input(LouiMultiTextInput widget) {
    auto widget_index = s_loui.text_input_widget_index_count++;
    auto is_selected = s_loui.active_text_input_widget_index == widget_index;
    auto keyboard = s_loui.keyboard_keys;
    if (is_selected) {
        if (s_loui.input_character) {
            widget.caret = insertCharacterMultiLineCaret(
                widget.caret,
                widget.text,
                LOUI_MAX_MULTI_LINE_TEXT_INPUT,
                s_loui.input_character
            );
        }
        if (isClicked(keyboard[LOUI_KEY_ENTER])) {
            widget.caret = insertLineBreakMultiLineCaret(
                widget.caret, widget.text, LOUI_MAX_MULTI_LINE_TEXT_INPUT
            );
        }
        if (isClicked(keyboard[LOUI_KEY_HOME])) {
            widget.caret = moveMultiLineCaretHome(widget.caret, widget.text);
        }
        if (isClicked(keyboard[LOUI_KEY_END])) {
            widget.caret = moveMultiLineCaretEnd(widget.caret, widget.text);
        }
        if (isClicked(keyboard[LOUI_KEY_PAGE_UP])) {
            widget.caret = moveMultiLineCaretPageUp(widget.caret, widget.text);
        }
        if (isClicked(keyboard[LOUI_KEY_PAGE_DOWN])) {
            widget.caret = moveMultiLineCaretPageDown(widget.caret, widget.text);
        }
        if (isClicked(keyboard[LOUI_KEY_ARROW_LEFT])) {
            widget.caret = moveMultiLineCaretLeft(widget.caret, widget.text);
        }
        if (isClicked(keyboard[LOUI_KEY_ARROW_RIGHT])) {
            widget.caret = moveMultiLineCaretRight(widget.caret, widget.text);
        }
        if (isClicked(keyboard[LOUI_KEY_ARROW_UP])) {
            widget.caret = moveMultiLineCaretUp(widget.caret, widget.text);
        }
        if (isClicked(keyboard[LOUI_KEY_ARROW_DOWN])) {
            widget.caret = moveMultiLineCaretDown(widget.caret, widget.text);
        }
        if (isClicked(keyboard[LOUI_KEY_DELETE])) {
            widget.caret = deleteCharacterAfterMultiLineCaret(widget.caret, widget.text);
        }
        if (isClicked(keyboard[LOUI_KEY_BACKSPACE])) {
            widget.caret = deleteCharacterBeforeMultiLineCaret(widget.caret, widget.text);
        }
        if (s_loui.mouse_wheel_y > 0) {
            for (auto i = 0; i < s_loui.mouse_wheel_y; ++i) {
                widget.caret = moveMultiLineCaretUp(widget.caret, widget.text);
            }
        }
        if (s_loui.mouse_wheel_y < 0) {
            for (auto i = 0; i < -s_loui.mouse_wheel_y; ++i) {
                widget.caret = moveMultiLineCaretDown(widget.caret, widget.text);
            }
        }
    }

    auto columns = widget.columns;
    auto lines = widget.lines;
    auto x = widget.x;
    auto y = widget.y;
    auto text_x = x + LOUI_BLOCK / 2;
    auto text_y = y + LOUI_BLOCK / 2;
    auto width = 8 * (widget.columns + 1) + LOUI_BLOCK;
    auto height = 8 * (widget.lines + 1) + LOUI_BLOCK;
    widget.width = width;
    widget.height = height;

    auto frame = (LouiSunkenFrame){.x=x, .y=y, .width=width, .height=height};
    frame = loui_update_sunken_frame(frame);

    widget.is_clicked = frame.is_clicked;
    if (widget.is_clicked) {
        s_loui.active_text_input_widget_index = widget_index;
        auto draw_column = (s_loui.mouse_x - text_x + TEXT_SIZE / 4) / TEXT_SIZE;
        auto draw_line = (s_loui.mouse_y - text_y) / TEXT_SIZE;
        auto column = draw_column + widget.draw_caret.column;
        auto line = draw_line + widget.draw_caret.line;
        widget.caret = moveMultiLineCaretLineColumn(widget.caret, widget.text, line, column);
    }

    // Handle caret moving too far above the draw caret:
    if (widget.draw_caret.line > widget.caret.line) {
        widget.draw_caret.line = widget.caret.line;
    }
    // Handle caret moving too far below draw caret:
    if (widget.draw_caret.line < widget.caret.line - widget.lines + 1) {
        widget.draw_caret.line = widget.caret.line - widget.lines + 1;
    }
    // Handle draw caret being below the text:
    if (widget.draw_caret.line > countLines(widget.text) - widget.lines) {
        widget.draw_caret.line = countLines(widget.text) - widget.lines;
    }
    // Handle draw caret being above the text:
    if (widget.draw_caret.line < 0) {
        widget.draw_caret.line = 0;
    }

    // Handle caret moving too far to the left of the draw caret:
    if (widget.draw_caret.column > widget.caret.column) {
        widget.draw_caret.column = widget.caret.column;
    }
    // Handle caret moving too far to the right of the draw caret:
    if (widget.draw_caret.column < widget.caret.column - widget.columns + 1) {
        widget.draw_caret.column = widget.caret.column - widget.columns + 1;
    }
    // Handle draw caret being on the left of the text:
    if (widget.draw_caret.column > countMaxColumns(widget.text) - widget.columns) {
        widget.draw_caret.column = countMaxColumns(widget.text) - widget.columns;
    }
    // Handle draw caret being on the right of the text:
    if (widget.draw_caret.column < 0) {
        widget.draw_caret.column = 0;
    }

    auto global_theme = s_loui.theme;
    auto local_theme = s_loui.theme;
    local_theme.text = is_selected ? local_theme.recess_text_selected : local_theme.recess_text;
    loui_set_theme(local_theme);
    drawMultiLineString(
        s_loui.screen,
        widget.text,
        text_x,
        text_y,
        s_loui.theme.text,
        lines,
        columns,
        widget.draw_caret
    );
    if (is_selected) {
        drawMultiLineCaret(
            s_loui.screen,
            text_x,
            text_y,
            s_loui.modifier_keys[LOUI_MODIFIER_KEY_SHIFT] == BUTTON_DOWN ? s_loui.theme.background : s_loui.theme.text,
            widget.caret,
            widget.draw_caret
        );
    }

    // Draw scrollbars:
    auto scroll_bar_thickness = 9;

    // Draw scrollbar checker backgrounds:
    auto rectangle_scroll_bar_vertical = (Rectangle){
        .x=widget.x + widget.width - scroll_bar_thickness,
        .y=widget.y + 2,
        .width=scroll_bar_thickness - 2,
        .height=widget.height - 4 - scroll_bar_thickness,
    };
    auto rectangle_scroll_bar_horizontal = (Rectangle){
        .x=widget.x + 2,
        .y=widget.y + widget.height - scroll_bar_thickness,
        .width=widget.width - 4 - scroll_bar_thickness,
        .height=scroll_bar_thickness - 2,
    };
    auto light = s_loui.theme.recess_text;
    auto dark = s_loui.theme.recess_background;
    drawCheckers(s_loui.screen, rectangle_scroll_bar_vertical, light, dark);
    drawCheckers(s_loui.screen, rectangle_scroll_bar_horizontal, light, dark);

    // Draw vertical scrollbar button:
    auto hidden_lines = countLines(widget.text) - widget.lines;
    if (hidden_lines > 0) {
        auto scrollbar_height = (widget.height - 2 - scroll_bar_thickness);
        auto scroll_button_height = scrollbar_height * widget.lines / countLines(widget.text);
        auto rectangle = (Rectangle){
            .x=widget.x + widget.width - scroll_bar_thickness - 1,
            .y=widget.y + 1 + widget.draw_caret.line * (scrollbar_height - scroll_button_height) / hidden_lines,
            .width=scroll_bar_thickness,
            .height=scroll_button_height
        };
        drawButton(rectangle, "");
    }
    // Draw horizontal scrollbar button:
    auto max_columns = countMaxColumns(widget.text);
    auto hidden_columns = max_columns - widget.columns;
    if (hidden_columns > 0) {
        auto scrollbar_width = (widget.width - 2 - scroll_bar_thickness);
        auto scroll_button_width = scrollbar_width * widget.columns / max_columns;
        auto rectangle = (Rectangle){
            .x=widget.x + 1 + widget.draw_caret.column * (scrollbar_width - scroll_button_width) / hidden_columns,
            .y=widget.y + widget.height - scroll_bar_thickness - 1,
            .width=scroll_button_width,
            .height=scroll_bar_thickness
        };
        drawButton(rectangle, "");
    }

    loui_set_theme(global_theme);

    return widget;
}
