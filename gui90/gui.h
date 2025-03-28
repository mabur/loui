#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "color.h"
#include "theme.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct GUI90_Label {
    int x;
    int y;
    int width;
    int height;
    bool is_clicked;
    const char* text;
} GUI90_Label;

typedef struct GUI90_HeaderLabel {
    int x;
    int y;
    int width;
    int height;
    bool is_clicked;
    const char* text;
    GUI90_HeaderLabelTheme theme;
} GUI90_HeaderLabel;


typedef struct GUI90_Button {
    int x;
    int y;
    int width;
    int height;
    bool is_clicked;
    const char* text;
} GUI90_Button;

typedef struct GUI90_RadioButton {
    int x;
    int y;
    int width;
    int height;
    bool is_clicked;
    const char* text;
    bool is_selected;
} GUI90_RadioButton;

typedef struct GUI90_TextInput {
    int x;
    int y;
    int width;
    int height;
    bool is_clicked;
    char text[16];
    int cursor;
} GUI90_TextInput;

typedef struct GUI90_Stepper {
    int x;
    int y;
    int width;
    int height;
    bool is_clicked;
    bool is_increased;
    bool is_decreased;
    const char* text;
} GUI90_Stepper;

typedef struct GUI90_SelectionBoxInit {
    int x;
    int y;
    int width;
    int height;
    bool is_clicked;
} GUI90_SelectionBoxInit;

typedef struct GUI90_SelectionBoxItem {
    int x;
    int y;
    int width;
    int height;
    bool is_clicked;
    const char* text;
    bool is_selected;
} GUI90_SelectionBoxItem;

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

void GUI90_WidgetBackground();
GUI90_Label GUI90_WidgetLabel(GUI90_Label widget);
GUI90_HeaderLabel GUI90_WidgetHeaderLabel(GUI90_HeaderLabel widget);
GUI90_Button GUI90_WidgetButton(GUI90_Button widget);
GUI90_RadioButton GUI90_WidgetRadioButton(GUI90_RadioButton widget);
GUI90_Stepper GUI90_WidgetStepper(GUI90_Stepper widget);

GUI90_SelectionBoxInit GUI90_WidgetSelectionBoxInit(GUI90_SelectionBoxInit widget);
GUI90_SelectionBoxItem GUI90_WidgetSelectionBoxItem(GUI90_SelectionBoxItem widget);

GUI90_TextInput GUI90_WidgetTextInput(GUI90_TextInput widget);

#ifdef __cplusplus
}
#endif
