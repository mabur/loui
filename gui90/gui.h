#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "color.h"
#include "theme.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct GUI90_Widget {
    int width;
    int height;
    bool is_clicked;
    bool is_increased;
    bool is_decreased;
} GUI90_Widget;

extern const int GUI90_BLOCK;

void GUI90_Init(int width, int height);
void GUI90_SetMouseState(int x, int y, bool is_left_mouse_button_down);
void GUI90_SetTheme(GUI90_Theme theme);
const GUI90_Color* GUI90_GetPixelData();

GUI90_Widget GUI90_WidgetBackground();
GUI90_Widget GUI90_WidgetLabel(int x, int y, const char* text);
GUI90_Widget GUI90_WidgetHeaderLabel(int x, int y, const char* text, GUI90_HeaderLabelTheme theme);
GUI90_Widget GUI90_WidgetButton(int x, int y, const char* text);
GUI90_Widget GUI90_WidgetRadioButton(int x, int y, const char* text, bool is_selected);
GUI90_Widget GUI90_WidgetStepper(int x, int y, const char* text);

GUI90_Widget GUI90_WidgetSelectionBoxInit(int x, int y, int width, int height);
GUI90_Widget GUI90_WidgetSelectionBoxItem(const char* text, bool is_selected);

GUI90_Widget GUI90_WidgetTextInput(int x, int y, const char* text);

#ifdef __cplusplus
}
#endif
