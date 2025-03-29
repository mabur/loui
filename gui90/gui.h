#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "color.h"
#include "theme.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GUI90_BLOCK 8
#define GUI90_MAX_SINGLE_LINE_TEXT_INPUT 16

typedef struct GUI90_Input {
    int mouse_x;
    int mouse_y;
    bool is_left_mouse_button_down;
    bool is_left_arrow_button_down;
    bool is_right_arrow_button_down;
    bool is_backspace_button_down;
    bool is_delete_button_down;
    bool is_home_button_down;
    bool is_end_button_down;
    char input_character;
} GUI90_Input;

typedef struct GUI90_Label {
    int x;
    int y;
    int width;
    int height;
    const char* text;
    bool is_clicked;
} GUI90_Label;

typedef struct GUI90_HeaderLabel {
    int x;
    int y;
    int width;
    int height;
    const char* text;
    bool is_clicked;
    GUI90_HeaderLabelTheme theme;
} GUI90_HeaderLabel;

typedef struct GUI90_SunkenFrame {
    int x;
    int y;
    int width;
    int height;
    bool is_clicked;
} GUI90_SunkenFrame;

typedef struct GUI90_Button {
    int x;
    int y;
    int width;
    int height;
    const char* text;
    bool is_clicked;
} GUI90_Button;

typedef struct GUI90_RadioButton {
    int x;
    int y;
    int width;
    int height;
    const char* text;
    bool is_selected;
    bool is_clicked;
} GUI90_RadioButton;

typedef struct GUI90_TextInput {
    int x;
    int y;
    int width;
    int height;
    char text[GUI90_MAX_SINGLE_LINE_TEXT_INPUT];
    int cursor;
    bool is_clicked;
} GUI90_TextInput;

typedef struct GUI90_Stepper {
    int x;
    int y;
    int width;
    int height;
    const char* text;
    bool is_clicked;
    bool is_increased;
    bool is_decreased;
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
    const char* text;
    bool is_selected;
    bool is_clicked;
} GUI90_SelectionBoxItem;

void GUI90_Init(int width, int height);
void GUI90_SetInput(GUI90_Input input);
void GUI90_SetTheme(GUI90_Theme theme);
const GUI90_Color* GUI90_GetPixelData();

void GUI90_WidgetBackground();

GUI90_Label GUI90_UpdateLabel(GUI90_Label widget);
GUI90_HeaderLabel GUI90_UpdateHeaderLabel(GUI90_HeaderLabel widget);
GUI90_Button GUI90_UpdateButton(GUI90_Button widget);
GUI90_RadioButton GUI90_UpdateRadioButton(GUI90_RadioButton widget);
GUI90_Stepper GUI90_UpdateStepper(GUI90_Stepper widget);
GUI90_SunkenFrame GUI90_UpdateSunkenFrame(GUI90_SunkenFrame widget);
GUI90_SelectionBoxInit GUI90_UpdateSelectionBoxInit(GUI90_SelectionBoxInit widget);
GUI90_SelectionBoxItem GUI90_UpdateSelectionBoxItem(GUI90_SelectionBoxItem widget);
GUI90_TextInput GUI90_UpdateTextInput(GUI90_TextInput widget);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
inline void GUI90_Update(GUI90_Label& widget) {widget = GUI90_UpdateLabel(widget);}
inline void GUI90_Update(GUI90_HeaderLabel& widget) {widget = GUI90_UpdateHeaderLabel(widget);}
inline void GUI90_Update(GUI90_Button& widget) {widget = GUI90_UpdateButton(widget);}
inline void GUI90_Update(GUI90_RadioButton& widget) {widget = GUI90_UpdateRadioButton(widget);}
inline void GUI90_Update(GUI90_Stepper& widget) {widget = GUI90_UpdateStepper(widget);}
inline void GUI90_Update(GUI90_SelectionBoxInit& widget) {widget = GUI90_UpdateSelectionBoxInit(widget);}
inline void GUI90_Update(GUI90_SelectionBoxItem& widget) {widget = GUI90_UpdateSelectionBoxItem(widget);}
inline void GUI90_Update(GUI90_TextInput& widget) {widget = GUI90_UpdateTextInput(widget);}
#else
#define GUI90_Update(widget) \
    do { widget = _Generic((widget), \
        GUI90_Label: GUI90_UpdateLabel, \
        GUI90_HeaderLabel: GUI90_UpdateHeaderLabel, \
        GUI90_Button: GUI90_UpdateButton, \
        GUI90_RadioButton: GUI90_UpdateRadioButton, \
        GUI90_Stepper: GUI90_UpdateStepper, \
        GUI90_SelectionBoxInit: GUI90_UpdateSelectionBoxInit, \
        GUI90_SelectionBoxItem: GUI90_UpdateSelectionBoxItem, \
        GUI90_TextInput: GUI90_UpdateTextInput \
    )(widget); } while(0)
#endif
