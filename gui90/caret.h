#pragma once

#include <stddef.h>

typedef struct SingleLineCaret {
    int column;
} SingleLineCaret;

typedef struct MultiLineCaret {
    int line;
    int column;
} MultiLineCaret;

// Move SingleLineCaret:
SingleLineCaret moveSingleLineCaretColumn(SingleLineCaret caret, const char* text, int column);
SingleLineCaret moveSingleLineCaretLeft(SingleLineCaret caret, const char* text);
SingleLineCaret moveSingleLineCaretRight(SingleLineCaret caret, const char* text);
SingleLineCaret moveSingleLineCaretHome(SingleLineCaret caret, const char* text);
SingleLineCaret moveSingleLineCaretEnd(SingleLineCaret caret, const char* text);
// Update text for SingleLineCaret:
SingleLineCaret insertCharacterSingleLineCaret(SingleLineCaret caret, char* text, size_t capacity, char c);
SingleLineCaret deleteCharacterAfterSingleLineCaret(SingleLineCaret caret, char* text);
SingleLineCaret deleteCharacterBeforeSingleLineCaret(SingleLineCaret caret, char* text);
// Move MultiLineCaret:
MultiLineCaret moveMultiLineCaretUp(MultiLineCaret caret, const char* text);
MultiLineCaret moveMultiLineCaretLeft(MultiLineCaret caret, const char* text);
MultiLineCaret moveMultiLineCaretDown(MultiLineCaret caret, const char* text);
MultiLineCaret moveMultiLineCaretRight(MultiLineCaret caret, const char* text);
MultiLineCaret moveMultiLineCaretHome(MultiLineCaret caret, const char* text);
MultiLineCaret moveMultiLineCaretEnd(MultiLineCaret caret, const char* text);
MultiLineCaret moveMultiLineCaretPageUp(MultiLineCaret caret, const char* text);
MultiLineCaret moveMultiLineCaretPageDown(MultiLineCaret caret, const char* text);
MultiLineCaret moveMultiLineCaretLineColumn(MultiLineCaret caret, const char* text, int line, int column);
// Update text for MultiLineCaret:
MultiLineCaret insertCharacterMultiLineCaret(MultiLineCaret caret, char* text, size_t capacity, char c);
MultiLineCaret insertLineBreakMultiLineCaret(MultiLineCaret caret, char* text, size_t capacity);
MultiLineCaret deleteCharacterAfterMultiLineCaret(MultiLineCaret caret, char* text);
MultiLineCaret deleteCharacterBeforeMultiLineCaret(MultiLineCaret caret, char* text);

int countLines(const char* text);
int countMaxColumns(const char* text);
