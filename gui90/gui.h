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

typedef struct GUI90_WidgetLabelType {
    int x;
    int y;
    int width;
    int height;
    bool is_clicked;
    const char* text;
} GUI90_WidgetLabelType;

typedef struct GUI90_WidgetHeaderLabelType {
    int x;
    int y;
    int width;
    int height;
    bool is_clicked;
    const char* text;
    GUI90_HeaderLabelTheme theme;
} GUI90_WidgetHeaderLabelType;


typedef struct GUI90_WidgetButtonType {
    int x;
    int y;
    int width;
    int height;
    bool is_clicked;
    const char* text;
} GUI90_WidgetButtonType;

typedef struct GUI90_WidgetRadioButtonType {
    int x;
    int y;
    int width;
    int height;
    bool is_clicked;
    const char* text;
    bool is_selected;
} GUI90_WidgetRadioButtonType;

typedef struct GUI90_WidgetText {
    int x;
    int y;
    int width;
    int height;
    bool is_clicked;
    char text[16];
    int cursor;
} GUI90_WidgetText;

typedef struct GUI90_WidgetStepperType {
    int x;
    int y;
    int width;
    int height;
    bool is_clicked;
    bool is_increased;
    bool is_decreased;
    const char* text;
} GUI90_WidgetStepperType;

extern const int GUI90_BLOCK;

void GUI90_Init(int width, int height);
void GUI90_SetMouseState(int x, int y, bool is_left_mouse_button_down);
void GUI90_SetKeyboardState(
    bool is_left_arrow_button_down,
    bool is_right_arrow_button_down,
    bool is_backspace_button_down,
    bool is_delete_button_down,
    char input_character
);
void GUI90_SetTheme(GUI90_Theme theme);
const GUI90_Color* GUI90_GetPixelData();

GUI90_Widget GUI90_WidgetBackground();
GUI90_WidgetLabelType GUI90_WidgetLabel(GUI90_WidgetLabelType widget);
GUI90_WidgetHeaderLabelType GUI90_WidgetHeaderLabel(GUI90_WidgetHeaderLabelType widget);
GUI90_WidgetButtonType GUI90_WidgetButton(GUI90_WidgetButtonType widget);
GUI90_WidgetRadioButtonType GUI90_WidgetRadioButton(GUI90_WidgetRadioButtonType widget);
GUI90_WidgetStepperType GUI90_WidgetStepper(GUI90_WidgetStepperType widget);

GUI90_Widget GUI90_WidgetSelectionBoxInit(int x, int y, int width, int height);
GUI90_Widget GUI90_WidgetSelectionBoxItem(const char* text, bool is_selected);

GUI90_WidgetText GUI90_WidgetTextInput(GUI90_WidgetText widget);

#ifdef __cplusplus
}
#endif
