#pragma once

#include <stddef.h>

#include "string.h"

typedef struct MultiLineCaret {
    int line;
    int column;
} MultiLineCaret;

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
MultiLineCaret insertCharactersMultiLineCaret(MultiLineCaret caret, StringBuilder text, StringRange clipboard);
MultiLineCaret insertCharacterMultiLineCaret(MultiLineCaret caret, char* text, size_t capacity, char c);
MultiLineCaret insertLineBreakMultiLineCaret(MultiLineCaret caret, char* text, size_t capacity);
MultiLineCaret deleteCharacterAfterMultiLineCaret(MultiLineCaret caret, char* text);
MultiLineCaret deleteCharacterBeforeMultiLineCaret(MultiLineCaret caret, char* text);
MultiLineCaret deleteSelectedCharactersMultiLineCaret(MultiLineCaret caret, MultiLineCaret selection_anchor, char* text);

int countLines(const char* text);
int countMaxColumns(const char* text);
int getIndexOfLineColumn(const char* text, int line, int column);

MultiLineCaret minMultiLineCaret(MultiLineCaret a, MultiLineCaret b);
MultiLineCaret maxMultiLineCaret(MultiLineCaret a, MultiLineCaret b);

bool isBetween(MultiLineCaret a, MultiLineCaret b, MultiLineCaret c);

bool equalMultiLineCaret(MultiLineCaret a, MultiLineCaret b);
