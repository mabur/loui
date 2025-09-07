#pragma once

#include "caret.h"
#include "state.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LouiMultiTextInput {
    int x;
    int y;
    int width;
    int height;
    char text[LOUI_MAX_MULTI_LINE_TEXT_INPUT];
    int lines;
    int columns;
    MultiLineCaret caret;
    MultiLineCaret draw_caret;
    MultiLineCaret selection_anchor;
    bool is_clicked;
} LouiMultiTextInput;

LouiMultiTextInput loui_update_multi_text_input(LouiMultiTextInput widget);

#ifdef __cplusplus
}
#endif
