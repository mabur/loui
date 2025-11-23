#include "loui_multi_text_input.h"

#include <stdio.h>

#include "draw.h"
#include "loui_button.h"
#include "loui_sunken_frame.h"
#include "rectangle.h"
#include "state.h"

static
void copySelection(const char* source, MultiLineCaret caret, MultiLineCaret selection_anchor, char* target){
    auto min = minMultiLineCaret(caret, selection_anchor);
    auto max = maxMultiLineCaret(caret, selection_anchor);

    auto i0 = (size_t)getIndexOfLineColumn(source, min.line, min.column);
    auto i1 = (size_t)getIndexOfLineColumn(source, max.line, max.column);

    auto count = i1 - i0;
    memcpy(target, source + i0, count);
    target[count] = '\0';
    printf("Clipboard: %s\n", target);
}

LouiMultiTextInput loui_update_multi_text_input(LouiMultiTextInput widget) {
    auto widget_index = s_loui.text_input_widget_index_count++;
    auto is_selected = s_loui.active_text_input_widget_index == widget_index;
    auto keyboard = s_loui.keyboard_keys;
    if (is_selected) {
        if (isControlDown()) {
            if (isClicked(keyboard[LOUI_KEY_C])) {
                copySelection(widget.text, widget.caret, widget.selection_anchor, s_loui.clipboard);
            }
            if (isClicked(keyboard[LOUI_KEY_V])) {
                widget.caret = insertCharactersMultiLineCaret(
                    widget.caret,
                    MAKE_STRING_BUILDER(widget.text),
                    MAKE_STRING_RANGE(s_loui.clipboard)
                );
                widget.selection_anchor = widget.caret;
            }
        }
        if (s_loui.input_character) {
            widget.caret = insertCharacterMultiLineCaret(
                widget.caret,
                widget.text,
                LOUI_MAX_MULTI_LINE_TEXT_INPUT,
                s_loui.input_character
            );
            widget.selection_anchor = widget.caret;
        }
        if (isClicked(keyboard[LOUI_KEY_ENTER])) {
            widget.caret = insertLineBreakMultiLineCaret(
                widget.caret, widget.text, LOUI_MAX_MULTI_LINE_TEXT_INPUT
            );
            widget.selection_anchor = widget.caret;
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
            if (equalMultiLineCaret(widget.caret, widget.selection_anchor)) {
                widget.caret = deleteCharacterAfterMultiLineCaret(widget.caret, widget.text);
            }
            else {
                widget.caret = deleteSelectedCharactersMultiLineCaret(widget.caret, widget.selection_anchor, widget.text);
            }
            widget.selection_anchor = widget.caret;
        }
        if (isClicked(keyboard[LOUI_KEY_BACKSPACE])) {
            if (equalMultiLineCaret(widget.caret, widget.selection_anchor)) {
                widget.caret = deleteCharacterBeforeMultiLineCaret(widget.caret, widget.text);
            }
            else {
                widget.caret = deleteSelectedCharactersMultiLineCaret(widget.caret, widget.selection_anchor, widget.text);
            }
            widget.selection_anchor = widget.caret;
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
    auto rectangle_scroll_bar_vertical = (LouiRectangle){
        .x=widget.x + widget.width - scroll_bar_thickness,
        .y=widget.y + 2,
        .width=scroll_bar_thickness - 2,
        .height=widget.height - 4 - scroll_bar_thickness,
    };
    auto rectangle_scroll_bar_horizontal = (LouiRectangle){
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
        auto rectangle = (LouiRectangle){
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
        auto rectangle = (LouiRectangle){
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
