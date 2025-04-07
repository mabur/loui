#pragma once

typedef struct MultiLineCaret {
    int cursor_row;
    int cursor_column;
} MultiLineCaret;

MultiLineCaret moveUpMultiLineCaret(MultiLineCaret caret, const char* text);
MultiLineCaret moveLeftMultiLineCaret(MultiLineCaret caret, const char* text);
MultiLineCaret moveDownMultiLineCaret(MultiLineCaret caret, const char* text);
MultiLineCaret moveRightMultiLineCaret(MultiLineCaret caret, const char* text);
