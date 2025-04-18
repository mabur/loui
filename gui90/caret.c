#include "caret.h"

#include <string.h>

static int countLines(const char* text) {
    auto lines = 1;
    for (; *text; ++text) {
        if (*text == '\n') {
            ++lines;
        }
    }
    return lines;
}

static const char* gotoNextLine(const char* text) {
    for (;; ++text) {
        if (*text == '\0') {
            return text;
        }
        if (*text == '\n') {
            ++text;
            return text;
        }
    }
}

static const char* gotoLine(const char* text, int line) {
    for (; line > 0; --line) {
        text = gotoNextLine(text);
    }
    return text;
}

static int countColumnsOfFirstLine(const char* text) {
    auto columns = 0;
    for (; *text && *text != '\n'; ++text, ++columns) {
    }
    return columns;
}

static int countColumns(const char* text, int line) {
    return countColumnsOfFirstLine(gotoLine(text, line));
}

static int getIndexOfLineColumn(const char* text, int line, int column) {
    auto line_start = gotoLine(text, line);
    auto position = line_start + column;
    return (int)(position - text);
}

static void insertCharacter(char* text, size_t index, char character) {
    size_t len = strlen(text);
    for (size_t i = len + 1; i > index; i--) {
        text[i] = text[i - 1];
    }
    text[index] = character;
}

static void deleteCharacter(char* text, int index) {
    auto count = (int)strlen(text);
    for (int i = index; i < count; ++i) {
        text[i] = text[i + 1];
    }
}

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
    if (caret.column > len) {
        return caret;
    }
    if (len + 1 >= capacity) {
        return caret;
    }
    insertCharacter(text, caret.column, c);
    return moveSingleLineCaretRight(caret, text);
}

SingleLineCaret deleteCharacterAfterSingleLineCaret(SingleLineCaret caret, char* text) {
    deleteCharacter(text, caret.column);
    return caret;
}

SingleLineCaret deleteCharacterBeforeSingleLineCaret(SingleLineCaret caret, char* text) {
    if (caret.column < 1) {
        return caret;
    }
    deleteCharacter(text, caret.column - 1);
    caret = moveSingleLineCaretLeft(caret, text);
    return caret;
}

MultiLineCaret moveMultiLineCaretUp(MultiLineCaret caret, const char* text) {
    if (caret.line > 0) {
        caret.line--;
        auto columns = countColumns(text, caret.line);
        if (caret.column > columns) {
            caret.column = columns;
        }
    }
    return caret;
}

MultiLineCaret moveMultiLineCaretLeft(MultiLineCaret caret, const char* text) {
    if (caret.column == 0) {
        if (caret.line > 0) {
            caret = moveMultiLineCaretUp(caret, text);
            caret.column = countColumns(text, caret.line) + 1;
        }
    }
    if (caret.column > 0) {
        caret.column--;
    }
    return caret;
}


MultiLineCaret moveMultiLineCaretDown(MultiLineCaret caret, const char* text) {
    if (caret.line < countLines(text) - 1) {
        caret.line++;
        auto columns = countColumns(text, caret.line);
        if (caret.column > columns) {
            caret.column = columns;
        }
    }
    return caret;
}

MultiLineCaret moveMultiLineCaretRight(MultiLineCaret caret, const char* text) {
    auto columns = countColumns(text, caret.line);
    if (caret.column == columns) {
        if (caret.line < countLines(text) - 1) {
            caret = moveMultiLineCaretDown(caret, text);
            caret.column = 0;
        }
        return caret;
    }
    if (caret.column < columns) {
        caret.column++;
    }
    return caret;
}

MultiLineCaret moveMultiLineCaretHome(MultiLineCaret caret, const char* text) {
    caret.column = 0;
    return caret;
}

MultiLineCaret moveMultiLineCaretEnd(MultiLineCaret caret, const char* text) {
    caret.column = countColumns(text, caret.line);
    return caret;
}

MultiLineCaret moveMultiLineCaretLineColumn(MultiLineCaret caret, const char* text, int line, int column) {
    auto lines = countLines(text);
    if (line < 0) {
        line = 0;
    }
    if (line > lines - 1) {
        line = lines - 1;
    }
    auto columns = countColumns(text, line);
    if (column < 0) {
        column = 0;
    }
    if (column > columns) {
        column = columns;
    }
    caret.line = line;
    caret.column = column;
    return caret;
}

MultiLineCaret insertCharacterMultiLineCaret(
    MultiLineCaret caret, char* text, size_t capacity, char c
) {
    auto index = getIndexOfLineColumn(text, caret.line, caret.column);
    size_t len = strlen(text);
    if (index > len) {
        return caret;
    }
    if (len + 1 >= capacity) {
        return caret;
    }
    insertCharacter(text, index, c);
    return moveMultiLineCaretRight(caret, text);
}
