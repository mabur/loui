#include "loui_text_input.h"

#include <stdio.h>
#include <string.h>

#include "draw.h"
#include "loui_label.h"
#include "loui_sunken_frame.h"
#include "rectangle.h"
#include "state.h"

void copySelection(const char* source, SingleLineCaret caret, SingleLineCaret selection_anchor, char* target){
    auto min = minSingleLineCaret(caret, selection_anchor);
    auto max = maxSingleLineCaret(caret, selection_anchor);
    auto count = max.column - min.column;
    memcpy(target, source + min.column, count);
    target[count] = '\0';
}

LouiTextInput loui_update_text_input(LouiTextInput widget) {
    auto widget_index = s_loui.text_input_widget_index_count++;
    auto is_selected = s_loui.active_text_input_widget_index == widget_index;
    auto keyboard = s_loui.keyboard_keys;
    if (is_selected) {
        if (isControlDown()) {
            if (isClicked(keyboard[LOUI_KEY_C])) {
                copySelection(widget.text, widget.caret, widget.selection_anchor, s_loui.clipboard);
            }
        }
        else if (s_loui.input_character) {
            widget.caret = insertCharacterSingleLineCaret(
                widget.caret,
                widget.text,
                LOUI_MAX_SINGLE_LINE_TEXT_INPUT,
                s_loui.input_character
            );
            widget.selection_anchor = widget.caret;
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
            if (widget.caret.column == widget.selection_anchor.column) {
                widget.caret = deleteCharacterAfterSingleLineCaret(widget.caret, widget.text);
            }
            else {
                widget.caret = deleteSelectedCharacters(widget.caret, widget.selection_anchor, widget.text);
            }
            widget.selection_anchor = widget.caret;
        }
        if (isClicked(keyboard[LOUI_KEY_BACKSPACE])) {
            if (widget.caret.column == widget.selection_anchor.column) {
                widget.caret = deleteCharacterBeforeSingleLineCaret(widget.caret, widget.text);
            }
            else {
                widget.caret = deleteSelectedCharacters(widget.caret, widget.selection_anchor, widget.text);
            }
            widget.selection_anchor = widget.caret;
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
