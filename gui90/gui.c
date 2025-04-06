#include "gui.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "text.h"

#define DRAW_DEBUG_RECTANGLES 0

typedef enum ButtonState {
    BUTTON_UP, BUTTON_CLICKED, BUTTON_DOWN, BUTTON_RELEASED
} ButtonState;

typedef struct RepeatingButtonState {
    ButtonState state;
    int frame_tick;
} RepeatingButtonState;

typedef struct LouiState {
    LouiColor* pixels;
    int width;
    int height;
    int mouse_x;
    int mouse_y;
    int current_x;
    int current_y;
    LouiTheme theme;
    ButtonState left_mouse_button;
    RepeatingButtonState left_arrow_button;
    RepeatingButtonState right_arrow_button;
    RepeatingButtonState backspace_button;
    RepeatingButtonState delete_button;
    ButtonState home_button;
    ButtonState end_button;
    char input_character;
    int active_text_input_widget_index;
    int text_input_widget_index_count;
} LouiState;

// -----------------------------------------------------------------------------
// PRIVATE STUFF

static const int TEXT_SIZE = 8;
static const int BUTTON_TEXT_PADDING = 4;

static LouiState s_loui;

typedef struct Rectangle {
    int x;
    int y;
    int width;
    int height;
} Rectangle;

// -----------------------------------------------------------------------------
// PRIVATE MOUSE & KEYBOARD FUNCTIONS

static ButtonState updateButtonState(ButtonState old_state, bool is_down) {
    switch (old_state) {
    case BUTTON_UP: return is_down ? BUTTON_CLICKED : BUTTON_UP;
    case BUTTON_CLICKED: return BUTTON_DOWN;
    case BUTTON_DOWN: return is_down ? BUTTON_DOWN : BUTTON_RELEASED;
    case BUTTON_RELEASED: return BUTTON_UP;
    default: return (ButtonState){};
    }
}

static bool is_key_repeating(int frame_tick) {
    auto INITIAL_REPEAT_TIME = 30;
    auto REPEAT_TIME = 3;
    if (frame_tick < INITIAL_REPEAT_TIME) {
        return false;
    }
    if (frame_tick == INITIAL_REPEAT_TIME) {
        return true;
    }
    return frame_tick % REPEAT_TIME == 0;
}

static RepeatingButtonState updateRepeatingButtonState(RepeatingButtonState old_state, bool is_down) {
    auto button = old_state;
    if (button.state == BUTTON_UP) {
        button.frame_tick = 0;
    }
    button.state = updateButtonState(button.state, is_down);
    if (button.state == BUTTON_DOWN) {
        button.frame_tick++;
        if (is_key_repeating(old_state.frame_tick)) {
            button.state = BUTTON_CLICKED;
        }
    }
    return button;
}

static bool isLeftMouseButtonDownInside(Rectangle r) {
    return s_loui.left_mouse_button == BUTTON_DOWN &&
           r.x <= s_loui.mouse_x && s_loui.mouse_x < r.x + r.width &&
           r.y <= s_loui.mouse_y && s_loui.mouse_y < r.y + r.height;
}

static bool isLeftMouseButtonReleasedInside(Rectangle r) {
    return s_loui.left_mouse_button == BUTTON_RELEASED &&
           r.x <= s_loui.mouse_x && s_loui.mouse_x < r.x + r.width &&
           r.y <= s_loui.mouse_y && s_loui.mouse_y < r.y + r.height;
}

// -----------------------------------------------------------------------------
// PRIVATE DRAW FUNCTIONS

static void drawPoint(int x, int y, LouiColor color) {
    s_loui.pixels[y * s_loui.width + x] = color;
}

static void drawRectangle(Rectangle rectangle, LouiColor color) {
    for (auto dy = 0; dy < rectangle.height; ++dy) {
        for (auto dx = 0; dx < rectangle.width; ++dx) {
            drawPoint(rectangle.x + dx, rectangle.y + dy, color);
        }
    }
}

static void drawLineHorizontal(int x, int y, int width, LouiColor color) {
    auto r = (Rectangle){};
    r.x = x;
    r.y = y;
    r.width = width;
    r.height = 1;
    drawRectangle(r, color);
}

static void drawLineVertical(int x, int y, int height, LouiColor color) {
    auto r = (Rectangle){};
    r.x = x;
    r.y = y;
    r.width = 1;
    r.height = height;
    drawRectangle(r, color);
}

LouiSunkenFrame loui_update_sunken_frame(LouiSunkenFrame widget) {
    auto x = widget.x;
    auto y = widget.y;
    auto width = widget.width;
    auto height = widget.height;
    auto rectangle = (Rectangle){x + 1, y + 1, width - 2, height - 2};
    drawRectangle(rectangle, s_loui.theme.recess_background);
    drawLineHorizontal(x + 1, y, width - 2, s_loui.theme.recess_bevel_dark);
    drawLineHorizontal(x + 1, y + height - 1, width - 2, s_loui.theme.recess_bevel_light);
    drawLineVertical(x, y + 1, height - 2, s_loui.theme.recess_bevel_dark);
    drawLineVertical(x + width - 1, y + 1, height - 2, s_loui.theme.recess_bevel_light);
    widget.is_clicked = isLeftMouseButtonReleasedInside(rectangle);
    return widget;
}

static void drawCursor(size_t x_start, size_t y_start, LouiColor color) {
    for (size_t y = 0; y < 8; ++y) {
        s_loui.pixels[(y_start + y) * s_loui.width + x_start] = color;
    }
}

static void drawCharacter(char character, size_t x_start, size_t y_start, LouiColor color) {
    auto W = s_loui.width;
    auto character_bitmap = character_bitmap8x8(character);
    for (size_t y = 0; y < 8; ++y) {
        for (size_t x = 0; x < 8; ++x) {
            if (character_bitmap[y * 8 + x]) {
                s_loui.pixels[(y_start + y) * W + x_start + x] = color;
            }
        }
    }
}

static void drawString(const char* s, size_t x, size_t y, LouiColor color) {
    for (; *s; ++s, x += 8) {
        drawCharacter(*s, x, y, color);
    }
}

static void drawSpecialString(const char* s, int x, int y, LouiHeaderLabelTheme theme) {
    for (; *s; ++s, x += 8) {
        if (theme.draw_up_left) {
            drawCharacter(*s, x - 1, y - 1, theme.color_up_left);
        }
        if (theme.draw_up_right) {
            drawCharacter(*s, x + 1, y - 1, theme.color_up_right);
        }
        if (theme.draw_down_left) {
            drawCharacter(*s, x - 1, y + 1, theme.color_down_left);
        }
        if (theme.draw_down_right) {
            drawCharacter(*s, x + 1, y + 1, theme.color_down_right);
        }
        
        if (theme.draw_up) {
            drawCharacter(*s, x + 0, y - 1, theme.color_up);
        }
        if (theme.draw_left) {
            drawCharacter(*s, x - 1, y + 0, theme.color_left);
        }
        if (theme.draw_right) {
            drawCharacter(*s, x + 1, y + 0, theme.color_right);
        }
        if (theme.draw_down) {
            drawCharacter(*s, x + 0, y + 1, theme.color_down);
        }
        
        if (theme.draw_center) {
            drawCharacter(*s, x + 0, y + 0, theme.color_center);
        }
    }
}

// -----------------------------------------------------------------------------
// PUBLIC FUNCTIONS

void loui_init(int width, int height) {
    if (s_loui.pixels != NULL) {
        free(s_loui.pixels);
    }
    s_loui = (LouiState){};
    s_loui.pixels = (LouiColor *)malloc(width * height * sizeof(LouiColor));
    s_loui.width = s_loui.pixels ? width : 0;
    s_loui.height = s_loui.pixels ? height : 0;
    s_loui.theme = LOUI_THEME_GRAY;
    s_loui.active_text_input_widget_index = -1;
}

void loui_set_input(LouiInput input) {
    // Mouse:
    s_loui.mouse_x = input.mouse_x;
    s_loui.mouse_y = input.mouse_y;
    s_loui.left_mouse_button = updateButtonState(s_loui.left_mouse_button, input.is_left_mouse_button_down);
    // Keyboard:
    s_loui.left_arrow_button = updateRepeatingButtonState(s_loui.left_arrow_button, input.is_left_arrow_button_down);
    s_loui.right_arrow_button = updateRepeatingButtonState(s_loui.right_arrow_button, input.is_right_arrow_button_down);
    s_loui.backspace_button = updateRepeatingButtonState(s_loui.backspace_button, input.is_backspace_button_down);
    s_loui.delete_button = updateRepeatingButtonState(s_loui.delete_button, input.is_delete_button_down);
    s_loui.home_button = updateButtonState(s_loui.home_button, input.is_home_button_down);
    s_loui.end_button = updateButtonState(s_loui.end_button, input.is_end_button_down);
    s_loui.input_character = input.input_character;
    // Other:
    s_loui.text_input_widget_index_count = 0;
}

void loui_set_theme(LouiTheme theme) {
    s_loui.theme = theme;
}

const LouiColor* loui_get_pixel_data() {
    return s_loui.pixels;
}

void loui_widget_background() {
    auto pixel_count = s_loui.width * s_loui.height;
    for (int i = 0; i < pixel_count; ++i) {
        s_loui.pixels[i] = s_loui.theme.background;
    }

    if (DRAW_DEBUG_RECTANGLES) {
        auto i = 0;
        for (auto y = 0; y < s_loui.height; ++y) {
            for (auto x = 0; x < s_loui.width; ++x, ++i) {
                if ((x / 8) % 2 != (y / 8 + 1) % 2) {
                    s_loui.pixels[i] = LOUI_RGB(0, 255, 0);
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

LouiLabel loui_update_label(LouiLabel widget) {
    drawString(widget.text, widget.x, widget.y, s_loui.theme.text);
    auto rectangle = textRectangle(widget.x, widget.y, widget.text);
    widget.width = rectangle.width;
    widget.height = rectangle.height;
    widget.is_clicked = isLeftMouseButtonReleasedInside(rectangle);
    return widget;
}

LouiHeaderLabel loui_update_header_label(LouiHeaderLabel widget) {
    drawSpecialString(widget.text, widget.x, widget.y, widget.theme);
    auto rectangle = textRectangle(widget.x, widget.y, widget.text);
    widget.width = rectangle.width;
    widget.height = rectangle.height;
    widget.is_clicked = isLeftMouseButtonReleasedInside(rectangle);
    return widget;
}

LouiButton loui_update_button_bevel(LouiButton widget) {
    auto x = widget.x;
    auto y = widget.y;
    x += 1;
    y += 1;
    auto rectangle = (Rectangle){};
    rectangle.x = x;
    rectangle.y = y;
    rectangle.width = 8 * (int)(strlen(widget.text)) + 6;
    rectangle.height = 8 + 6;
    auto inner_rectangle = rectangle;
    inner_rectangle.x += 2;
    inner_rectangle.y += 2;
    inner_rectangle.width -= 4;
    inner_rectangle.height -= 4;
    auto text_x = widget.x + BUTTON_TEXT_PADDING;
    auto text_y = widget.y + BUTTON_TEXT_PADDING;
    auto theme = s_loui.theme;
    if (isLeftMouseButtonDownInside(rectangle)) {
        ++text_y;
        theme.button_bevel_light = s_loui.theme.button_background;
        theme.button_bevel_dark = s_loui.theme.button_background;
    }
    
    drawRectangle(rectangle, s_loui.theme.button_border);
    
    // Rounded corners:
    // drawLineHorizontal(rectangle.x + 1, rectangle.y, rectangle.width - 2, theme.button_border);
    // drawLineHorizontal(rectangle.x + 1, rectangle.y + rectangle.height - 1, rectangle.width - 2, theme.button_border);
    // drawLineVertical(rectangle.x, rectangle.y + 1, rectangle.height - 2, theme.button_border);
    // drawLineVertical(rectangle.x + rectangle.width - 1, rectangle.y + 1, rectangle.height - 2, theme.button_border);
    // drawPoint(rectangle.x + 1, rectangle.y + 1, theme.button_border);
    // drawPoint(rectangle.x + 1, rectangle.y + rectangle.height - 2, theme.button_border);
    // drawPoint(rectangle.x + + rectangle.width - 2, rectangle.y + 1, theme.button_border);
    // drawPoint(rectangle.x + + rectangle.width - 2, rectangle.y + rectangle.height - 2, theme.button_border);
    
    drawRectangle(inner_rectangle, theme.button_background);
    drawLineHorizontal(x + 2, y + 1, rectangle.width - 4, theme.button_bevel_light);
    drawLineHorizontal(x + 2, y + rectangle.height - 2, rectangle.width - 4, theme.button_bevel_dark);
    drawLineVertical(x + 1, y + 2, rectangle.height - 4, theme.button_bevel_light);
    drawLineVertical(x + rectangle.width - 2, y + 2, rectangle.height - 4, theme.button_bevel_dark);
    drawString(widget.text, text_x, text_y, theme.text);

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
    
    auto inner_rectangle = rectangle;
    inner_rectangle.x += 1;
    inner_rectangle.y += 1;
    inner_rectangle.width -= 2;
    inner_rectangle.height -= 2;
    auto text_x = x + BUTTON_TEXT_PADDING - 1;
    auto text_y = y + BUTTON_TEXT_PADDING - 2;
    auto theme = s_loui.theme;
    if (isLeftMouseButtonDownInside(rectangle)) {
        ++text_y;
    }

    // Shadow:
    drawPoint(x, y + rectangle.height - 1, theme.button_border);
    drawPoint(x + rectangle.width - 1, y + rectangle.height - 1, theme.button_border);
    drawLineHorizontal(x + 1, y + rectangle.height, rectangle.width - 2, theme.button_border);
    
    //drawRectangle(rectangle, s_loui.theme.button_border);
    drawRectangle(inner_rectangle, theme.button_background);
    
    // Rounded corners:
    drawLineHorizontal(rectangle.x + 1, rectangle.y, rectangle.width - 2, theme.button_border);
    drawLineHorizontal(rectangle.x + 1, rectangle.y + rectangle.height - 1, rectangle.width - 2, theme.button_border);
    drawLineVertical(rectangle.x, rectangle.y + 1, rectangle.height - 2, theme.button_border);
    drawLineVertical(rectangle.x + rectangle.width - 1, rectangle.y + 1, rectangle.height - 2, theme.button_border);
    drawPoint(rectangle.x + 1, rectangle.y + 1, theme.button_border);
    drawPoint(rectangle.x + 1, rectangle.y + rectangle.height - 2, theme.button_border);
    drawPoint(rectangle.x + + rectangle.width - 2, rectangle.y + 1, theme.button_border);
    drawPoint(rectangle.x + + rectangle.width - 2, rectangle.y + rectangle.height - 2, theme.button_border);
    
    drawPoint(rectangle.x + 2, rectangle.y + 1, theme.button_bevel_dark);
    drawPoint(rectangle.x + 1, rectangle.y + 2, theme.button_bevel_dark);
    drawPoint(rectangle.x + 1, rectangle.y + rectangle.height - 3, theme.button_bevel_dark);
    drawPoint(rectangle.x + 2, rectangle.y + rectangle.height - 2, theme.button_bevel_dark);
    drawPoint(rectangle.x + rectangle.width - 3, rectangle.y + 1, theme.button_bevel_dark);
    drawPoint(rectangle.x + rectangle.width - 2, rectangle.y + 2, theme.button_bevel_dark);
    drawPoint(rectangle.x + rectangle.width - 3, rectangle.y + rectangle.height - 2, theme.button_bevel_dark);
    drawPoint(rectangle.x + rectangle.width - 2, rectangle.y + rectangle.height - 3, theme.button_bevel_dark);
    
    drawString(widget.text, text_x, text_y, theme.text);
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
            drawPoint(widget.x + xi, widget.y + yi, color);
        }
    }
    auto left_rectangle = (Rectangle){widget.x, widget.y, 16 + BUTTON_TEXT_PADDING, 16};
    auto label = (LouiLabel){
        .x=widget.x + 16 + BUTTON_TEXT_PADDING,
        .y=widget.y + BUTTON_TEXT_PADDING,
        .text=widget.text
    };
    auto label_result = loui_update_label(label);
    widget.width = label_result.width + 16 + 8;
    widget.height = 16;
    widget.is_clicked = label_result.is_clicked || isLeftMouseButtonReleasedInside(left_rectangle);
    return widget;
}

LouiCheckBox loui_update_check_box(LouiCheckBox widget) {
    auto x = widget.x;
    auto y = widget.y;
    auto box_size = 10;
    auto frame = (LouiSunkenFrame){
        .x=x + (16 - box_size) / 2,
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
                    drawPoint(x + 4 + dx, y + 4 + dy, s_loui.theme.recess_text_selected);
                }
            }
        }
    }

    auto left_rectangle = (Rectangle){widget.x, widget.y, 16 + BUTTON_TEXT_PADDING, 16};
    auto label = (LouiLabel){
        .x=widget.x + 16 + BUTTON_TEXT_PADDING,
        .y=widget.y + BUTTON_TEXT_PADDING,
        .text=widget.text
    };
    auto label_result = loui_update_label(label);
    widget.width = label_result.width + 16 + 8;
    widget.height = 16;
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

static LouiTextInput decrementCursor(LouiTextInput widget) {
    if (widget.cursor > 0) {
        widget.cursor--;
    }
    return widget;
}

static LouiMultiTextInput decrementCursorMulti(LouiMultiTextInput widget) {
    if (widget.cursor > 0) {
        widget.cursor--;
    }
    return widget;
}

static LouiTextInput incrementCursor(LouiTextInput widget) {
    if (widget.cursor < strlen(widget.text)) {
        widget.cursor++;
    }
    return widget;
}

static LouiMultiTextInput incrementCursorMulti(LouiMultiTextInput widget) {
    if (widget.cursor < strlen(widget.text)) {
        widget.cursor++;
    }
    return widget;
}

static void deleteCharacter(char* string, size_t index) {
    auto len = strlen(string);
    for (size_t i = index; i < len; ++i) {
        string[i] = string[i + 1];
    }
}

static void insertCharacter(char* string, size_t index, char c) {
    size_t len = strlen(string);
    size_t max_size = LOUI_MAX_SINGLE_LINE_TEXT_INPUT;
    if (len + 1 >= max_size || index > len) {
        return;
    }
    for (size_t i = len + 1; i > index; i--) {
        string[i] = string[i - 1];
    }
    string[index] = c;
}

LouiTextInput loui_update_text_input(LouiTextInput widget) {
    auto widget_index = s_loui.text_input_widget_index_count++;
    auto is_selected = s_loui.active_text_input_widget_index == widget_index;
    if (is_selected) {
        if (s_loui.input_character && strlen(widget.text) < LOUI_MAX_SINGLE_LINE_TEXT_INPUT - 1) {
            insertCharacter(widget.text, widget.cursor, s_loui.input_character);
            widget = incrementCursor(widget);
        }
        if (s_loui.home_button == BUTTON_CLICKED) {
            widget.cursor = 0;
        }
        if (s_loui.end_button == BUTTON_CLICKED) {
            widget.cursor = strlen(widget.text);
        }
        if (s_loui.left_arrow_button.state == BUTTON_CLICKED) {
            widget = decrementCursor(widget);
        }
        if (s_loui.right_arrow_button.state == BUTTON_CLICKED) {
            widget = incrementCursor(widget);
        }
        if (s_loui.delete_button.state == BUTTON_CLICKED) {
            deleteCharacter(widget.text, widget.cursor);
        }
        if (s_loui.backspace_button.state == BUTTON_CLICKED && widget.cursor > 0) {
            deleteCharacter(widget.text, widget.cursor - 1);
            widget = decrementCursor(widget);
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

        widget.cursor = (s_loui.mouse_x - text_x + TEXT_SIZE / 4) / TEXT_SIZE;
        if (widget.cursor < 0) {
            widget.cursor = 0;
        }
        if (widget.cursor > strlen(widget.text)) {
            widget.cursor = strlen(widget.text);
        }
    }

    auto global_theme = s_loui.theme;
    auto local_theme = s_loui.theme;
    local_theme.text = is_selected ? local_theme.recess_text_selected : local_theme.recess_text;
    loui_set_theme(local_theme);
    drawString(widget.text, text_x, text_y, s_loui.theme.text);
    if (is_selected) {
        auto cursor_x = text_x + widget.cursor * TEXT_SIZE;
        auto cursor_y = text_y;
        drawCursor(cursor_x, cursor_y, s_loui.theme.text);
    }
    loui_set_theme(global_theme);

    return widget;
}

LouiMultiTextInput loui_update_multi_text_input(LouiMultiTextInput widget) {
    auto widget_index = s_loui.text_input_widget_index_count++;
    auto is_selected = s_loui.active_text_input_widget_index == widget_index;
    if (is_selected) {
        if (s_loui.input_character && strlen(widget.text) < LOUI_MAX_MULTI_LINE_TEXT_INPUT - 1) {
            insertCharacter(widget.text, widget.cursor, s_loui.input_character);
            widget = incrementCursorMulti(widget);
        }
        if (s_loui.home_button == BUTTON_CLICKED) {
            widget.cursor = 0;
        }
        if (s_loui.end_button == BUTTON_CLICKED) {
            widget.cursor = strlen(widget.text);
        }
        if (s_loui.left_arrow_button.state == BUTTON_CLICKED) {
            widget = decrementCursorMulti(widget);
        }
        if (s_loui.right_arrow_button.state == BUTTON_CLICKED) {
            widget = incrementCursorMulti(widget);
        }
        if (s_loui.delete_button.state == BUTTON_CLICKED) {
            deleteCharacter(widget.text, widget.cursor);
        }
        if (s_loui.backspace_button.state == BUTTON_CLICKED && widget.cursor > 0) {
            deleteCharacter(widget.text, widget.cursor - 1);
            widget = incrementCursorMulti(widget);
        }
    }

    auto x = widget.x;
    auto y = widget.y;
    auto text_x = x + LOUI_BLOCK / 2;
    auto text_y = y + LOUI_BLOCK / 2;
    auto width = 8 * widget.columns + LOUI_BLOCK;
    auto height = 8 * widget.rows + LOUI_BLOCK;
    widget.width = width;
    widget.height = height;

    auto frame = (LouiSunkenFrame){.x=x, .y=y, .width=width, .height=height};
    frame = loui_update_sunken_frame(frame);

    widget.is_clicked = frame.is_clicked;
    if (widget.is_clicked) {
        s_loui.active_text_input_widget_index = widget_index;

        widget.cursor = (s_loui.mouse_x - text_x + TEXT_SIZE / 4) / TEXT_SIZE;
        if (widget.cursor < 0) {
            widget.cursor = 0;
        }
        if (widget.cursor > strlen(widget.text)) {
            widget.cursor = strlen(widget.text);
        }
    }

    auto global_theme = s_loui.theme;
    auto local_theme = s_loui.theme;
    local_theme.text = is_selected ? local_theme.recess_text_selected : local_theme.recess_text;
    loui_set_theme(local_theme);
    drawString(widget.text, text_x, text_y, s_loui.theme.text);
    if (is_selected) {
        auto cursor_x = text_x + widget.cursor * TEXT_SIZE;
        auto cursor_y = text_y;
        drawCursor(cursor_x, cursor_y, s_loui.theme.text);
    }
    loui_set_theme(global_theme);

    return widget;
}
