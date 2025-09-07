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

LouiWindow loui_update_window(LouiWindow widget) {
    auto x = widget.x;
    auto y = widget.y;
    auto width = widget.width;
    auto height = widget.height;
    auto rectangle = (Rectangle){x, y, width, height};
    auto inner_rectangle = shrinkRectangle(rectangle);
    auto innermost_rectangle = shrinkRectangle(inner_rectangle);

    auto border_color = getTheme().button_border;
    auto background_color = getTheme().background;
    auto light_bevel_color = getTheme().recess_bevel_light;
    auto dark_bevel_color = getTheme().recess_bevel_dark;

    drawRoundedRectangleOutline(s_loui.screen, rectangle, border_color, border_color);
    drawRoundedRectangleOutline(s_loui.screen, inner_rectangle, light_bevel_color, dark_bevel_color);
    drawRectangleCorners(s_loui.screen, inner_rectangle, border_color);
    drawRectangle(s_loui.screen, innermost_rectangle, background_color);

    widget.is_clicked = isLeftMouseButtonReleasedInside(innermost_rectangle);
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
                    drawPoint(s_loui.screen, x + 1 + dx, y + 4 + dy, getTheme().recess_text_selected);
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

static bool isShiftUp() {
    return s_loui.modifier_keys[LOUI_MODIFIER_KEY_SHIFT] == BUTTON_UP;
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
        // Navigation:
        if (isClicked(keyboard[LOUI_KEY_HOME])) {
            widget.caret = moveSingleLineCaretHome(widget.caret, widget.text);
            if (isShiftUp())
                widget.selection_anchor = widget.caret;
        }
        if (isClicked(keyboard[LOUI_KEY_END])) {
            widget.caret = moveSingleLineCaretEnd(widget.caret, widget.text);
            if (isShiftUp())
                widget.selection_anchor = widget.caret;
        }
        if (isClicked(keyboard[LOUI_KEY_ARROW_LEFT])) {
            widget.caret = moveSingleLineCaretLeft(widget.caret, widget.text);
            if (isShiftUp())
                widget.selection_anchor = widget.caret;
        }
        if (isClicked(keyboard[LOUI_KEY_ARROW_RIGHT])) {
            widget.caret = moveSingleLineCaretRight(widget.caret, widget.text);
            if (isShiftUp())
                widget.selection_anchor = widget.caret;
        }
        // Deleting characters:
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
        widget.caret = moveSingleLineCaretColumn(widget.caret, widget.text, column);
        if (isShiftUp())
            widget.selection_anchor = widget.caret;
    }

    auto global_theme = getTheme();
    auto local_theme = getTheme();
    local_theme.text = is_selected ? local_theme.recess_text_selected : local_theme.recess_text;
    loui_set_theme(local_theme);

    auto selection_begin = minSingleLineCaret(widget.caret, widget.selection_anchor);
    auto selection_end = maxSingleLineCaret(widget.caret, widget.selection_anchor);
    auto selection = (Rectangle){
        .x = text_x + selection_begin.column * TEXT_SIZE,
        .y = text_y,
        .width = (selection_end.column - selection_begin.column) * TEXT_SIZE,
        .height = TEXT_SIZE,
    };
    drawRectangle(s_loui.screen, selection, getTheme().background);

    drawString(s_loui.screen, widget.text, text_x, text_y, getTheme().text);
    if (is_selected) {
        auto cursor_x = text_x + widget.caret.column * TEXT_SIZE;
        auto cursor_y = text_y;
        drawCaret(
            s_loui.screen,
            cursor_x,
            cursor_y,
            getTheme().text
        );
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
        // Navigation:
        if (isClicked(keyboard[LOUI_KEY_HOME])) {
            widget.caret = moveMultiLineCaretHome(widget.caret, widget.text);
            if (isShiftUp())
                widget.selection_anchor = widget.caret;
        }
        if (isClicked(keyboard[LOUI_KEY_END])) {
            widget.caret = moveMultiLineCaretEnd(widget.caret, widget.text);
            if (isShiftUp())
                widget.selection_anchor = widget.caret;
        }
        if (isClicked(keyboard[LOUI_KEY_PAGE_UP])) {
            widget.caret = moveMultiLineCaretPageUp(widget.caret, widget.text);
            if (isShiftUp())
                widget.selection_anchor = widget.caret;
        }
        if (isClicked(keyboard[LOUI_KEY_PAGE_DOWN])) {
            widget.caret = moveMultiLineCaretPageDown(widget.caret, widget.text);
            if (isShiftUp())
                widget.selection_anchor = widget.caret;
        }
        if (isClicked(keyboard[LOUI_KEY_ARROW_LEFT])) {
            widget.caret = moveMultiLineCaretLeft(widget.caret, widget.text);
            if (isShiftUp())
                widget.selection_anchor = widget.caret;
        }
        if (isClicked(keyboard[LOUI_KEY_ARROW_RIGHT])) {
            widget.caret = moveMultiLineCaretRight(widget.caret, widget.text);
            if (isShiftUp())
                widget.selection_anchor = widget.caret;
        }
        if (isClicked(keyboard[LOUI_KEY_ARROW_UP])) {
            widget.caret = moveMultiLineCaretUp(widget.caret, widget.text);
            if (isShiftUp())
                widget.selection_anchor = widget.caret;
        }
        if (isClicked(keyboard[LOUI_KEY_ARROW_DOWN])) {
            widget.caret = moveMultiLineCaretDown(widget.caret, widget.text);
            if (isShiftUp())
                widget.selection_anchor = widget.caret;
        }
        // Deleting characters:
        if (isClicked(keyboard[LOUI_KEY_DELETE])) {
            widget.caret = deleteCharacterAfterMultiLineCaret(widget.caret, widget.text);
        }
        if (isClicked(keyboard[LOUI_KEY_BACKSPACE])) {
            widget.caret = deleteCharacterBeforeMultiLineCaret(widget.caret, widget.text);
        }
        // Scrolling:
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
        if (isShiftUp())
            widget.selection_anchor = widget.caret;
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

    auto global_theme = getTheme();
    auto local_theme = getTheme();
    local_theme.text = is_selected ? local_theme.recess_text_selected : local_theme.recess_text;
    loui_set_theme(local_theme);
    drawMultiLineString(
        s_loui.screen,
        widget.text,
        text_x,
        text_y,
        getTheme().text,
        getTheme().background,
        lines,
        columns,
        widget.caret,
        widget.selection_anchor,
        widget.draw_caret
    );
    if (is_selected) {
        drawMultiLineCaret(
            s_loui.screen,
            text_x,
            text_y,
            getTheme().text,
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
    auto light = getTheme().recess_text;
    auto dark = getTheme().recess_background;
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
