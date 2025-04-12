#include "caret.h"

#include <string.h>

#include "string.h"

SingleLineCaret moveSingleLineCaretColumn(SingleLineCaret caret, const char* text, int column) {
    auto count = (int)strlen(text);
    if (column < 0) {
        column = 0;
    }
    if (column > count) {
        column = count;
    }
    caret.column = column;
    return caret;
}

SingleLineCaret moveSingleLineCaretLeft(SingleLineCaret caret, const char* text) {
    return moveSingleLineCaretColumn(caret, text, caret.column - 1);
}

SingleLineCaret moveSingleLineCaretRight(SingleLineCaret caret, const char* text) {
    return moveSingleLineCaretColumn(caret, text, caret.column + 1);
}

SingleLineCaret moveSingleLineCaretHome(SingleLineCaret caret, const char* text) {
    return moveSingleLineCaretColumn(caret, text, 0);
}

SingleLineCaret moveSingleLineCaretEnd(SingleLineCaret caret, const char* text) {
    return moveSingleLineCaretColumn(caret, text, strlen(text));
}

SingleLineCaret insertCharacterSingleLineCaret(SingleLineCaret caret, char* text, size_t capacity, char c) {
    size_t len = strlen(text);
    if (len + 1 >= capacity) {
        return caret;
    }
    size_t max_size = capacity;
    if (len + 1 >= max_size || caret.column > len) {
        return caret;
    }
    for (size_t i = len + 1; i > caret.column; i--) {
        text[i] = text[i - 1];
    }
    text[caret.column] = c;
    return moveSingleLineCaretRight(caret, text);
}

SingleLineCaret deleteCharacterAfterSingleLineCaret(SingleLineCaret caret, char* text) {
    auto count = (int)strlen(text);
    for (int i = caret.column; i < count; ++i) {
        text[i] = text[i + 1];
    }
    return caret;
}

SingleLineCaret deleteCharacterBeforeSingleLineCaret(SingleLineCaret caret, char* text) {
    if (caret.column < 1) {
        return caret;
    }
    auto count = (int)strlen(text);
    for (int i = caret.column - 1; i < count; ++i) {
        text[i] = text[i + 1];
    }
    caret = moveSingleLineCaretLeft(caret, text);
    return caret;
}

MultiLineCaret moveUpMultiLineCaret(MultiLineCaret caret, const char* text) {
    if (caret.line > 0) {
        caret.line--;
        auto columns = countColumns(text, caret.line);
        if (caret.column > columns) {
            caret.column = columns;
        }
    }
    return caret;
}


MultiLineCaret moveLeftMultiLineCaret(MultiLineCaret caret, const char* text) {
    if (caret.column == 0) {
        if (caret.line > 0) {
            caret = moveUpMultiLineCaret(caret, text);
            caret.column = countColumns(text, caret.line) + 1;
        }
    }
    if (caret.column > 0) {
        caret.column--;
    }
    return caret;
}


MultiLineCaret moveDownMultiLineCaret(MultiLineCaret caret, const char* text) {
    if (caret.line < countRows(text)) {
        caret.line++;
        auto columns = countColumns(text, caret.line);
        if (caret.column > columns) {
            caret.column = columns;
        }
    }
    return caret;
}

MultiLineCaret moveRightMultiLineCaret(MultiLineCaret caret, const char* text) {
    auto columns = countColumns(text, caret.line);
    if (caret.column == columns) {
        if (caret.line < countRows(text)) {
            caret = moveDownMultiLineCaret(caret, text);
            caret.column = 0;
        }
        return caret;
    }
    if (caret.column < columns) {
        caret.column++;
    }
    return caret;
}
