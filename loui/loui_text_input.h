#pragma once

#include "caret.h"
#include "state.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LouiTextInput {
    int x;
    int y;
    int width;
    int height;
    char text[LOUI_MAX_SINGLE_LINE_TEXT_INPUT];
    SingleLineCaret caret;
    SingleLineCaret selection_anchor;
    bool is_clicked;
} LouiTextInput;

LouiTextInput loui_update_text_input(LouiTextInput widget);

#ifdef __cplusplus
}
#endif
