#pragma once

#include <stddef.h>

typedef struct SingleLineCaret {
    int column;
} SingleLineCaret;

typedef struct MultiLineCaret {
    int line;
    int column;
} MultiLineCaret;

SingleLineCaret moveLeftSingleLineCaret(SingleLineCaret caret);
SingleLineCaret moveRightSingleLineCaret(SingleLineCaret caret, const char* text);
SingleLineCaret moveHomeSingleLineCaret(SingleLineCaret caret);
SingleLineCaret moveEndSingleLineCaret(SingleLineCaret caret, const char* text);
SingleLineCaret insertCharacterSingleLineCaret(SingleLineCaret caret, char* text, size_t capacity, char c);
SingleLineCaret deleteCharacterAfterSingleLineCaret(SingleLineCaret caret, char* text);
SingleLineCaret deleteCharacterBeforeSingleLineCaret(SingleLineCaret caret, char* text);

MultiLineCaret moveUpMultiLineCaret(MultiLineCaret caret, const char* text);
MultiLineCaret moveLeftMultiLineCaret(MultiLineCaret caret, const char* text);
MultiLineCaret moveDownMultiLineCaret(MultiLineCaret caret, const char* text);
MultiLineCaret moveRightMultiLineCaret(MultiLineCaret caret, const char* text);
