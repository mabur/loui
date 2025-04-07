#include "caret.h"

#include "string.h"

MultiLineCaret moveUpMultiLineCaret(MultiLineCaret caret, const char* text) {
    if (caret.cursor_row > 0) {
        caret.cursor_row--;
        auto columns = countColumns(text, caret.cursor_row);
        if (caret.cursor_column > columns) {
            caret.cursor_column = columns;
        }
    }
    return caret;
}


MultiLineCaret moveLeftMultiLineCaret(MultiLineCaret caret, const char* text) {
    if (caret.cursor_column == 0) {
        if (caret.cursor_row > 0) {
            caret = moveUpMultiLineCaret(caret, text);
            caret.cursor_column = countColumns(text, caret.cursor_row) + 1;
        }
    }
    if (caret.cursor_column > 0) {
        caret.cursor_column--;
    }
    return caret;
}


MultiLineCaret moveDownMultiLineCaret(MultiLineCaret caret, const char* text) {
    if (caret.cursor_row < countRows(text)) {
        caret.cursor_row++;
        auto columns = countColumns(text, caret.cursor_row);
        if (caret.cursor_column > columns) {
            caret.cursor_column = columns;
        }
    }
    return caret;
}

MultiLineCaret moveRightMultiLineCaret(MultiLineCaret caret, const char* text) {
    auto columns = countColumns(text, caret.cursor_row);
    if (caret.cursor_column == columns) {
        if (caret.cursor_row < countRows(text)) {
            caret = moveDownMultiLineCaret(caret, text);
            caret.cursor_column = 0;
        }
        return caret;
    }
    if (caret.cursor_column < columns) {
        caret.cursor_column++;
    }
    return caret;
}
