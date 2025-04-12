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
SingleLineCaret moveSingleLineCaretLeft(SingleLineCaret caret, const char* text);
SingleLineCaret moveSingleLineCaretRight(SingleLineCaret caret, const char* text);
SingleLineCaret moveSingleLineCaretHome(SingleLineCaret caret, const char* text);
SingleLineCaret moveSingleLineCaretEnd(SingleLineCaret caret, const char* text);
SingleLineCaret moveSingleLineCaretColumn(SingleLineCaret caret, const char* text, int column);
// Update text for SingleLineCaret:
SingleLineCaret insertCharacterSingleLineCaret(SingleLineCaret caret, char* text, size_t capacity, char c);
SingleLineCaret deleteCharacterAfterSingleLineCaret(SingleLineCaret caret, char* text);
SingleLineCaret deleteCharacterBeforeSingleLineCaret(SingleLineCaret caret, char* text);

MultiLineCaret moveUpMultiLineCaret(MultiLineCaret caret, const char* text);
MultiLineCaret moveLeftMultiLineCaret(MultiLineCaret caret, const char* text);
MultiLineCaret moveDownMultiLineCaret(MultiLineCaret caret, const char* text);
MultiLineCaret moveRightMultiLineCaret(MultiLineCaret caret, const char* text);
