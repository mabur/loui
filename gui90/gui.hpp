#pragma once

#include <stdint.h>

typedef uint32_t Color;

typedef struct {
    Color text;
    Color text_selection_box_selected;
    Color text_selection_box;
    Color background;
    Color background_dark;
    Color bevel_light;
    Color bevel_dark;
    Color border;
} GUI90_Theme;

extern const GUI90_Theme GUI90_THEME_YELLOW;
extern const GUI90_Theme GUI90_THEME_GRAY;
extern const GUI90_Theme GUI90_THEME_LEATHER;

void GUI90_Init(int width, int height);
void GUI90_SetMouseState(int x, int y, bool is_left_mouse_button_down);
const Color* GUI90_GetPixelData();

void GUI90_WidgetBackground(GUI90_Theme theme);
bool GUI90_WidgetLabel(int x, int y, const char* text, GUI90_Theme theme);
bool GUI90_WidgetButton(int x, int y, const char* text, GUI90_Theme theme);
int GUI90_WidgetIntSetting(int x, int y, const char* text, int value, int min_value, int max_value, GUI90_Theme label_colors, GUI90_Theme button_colors);

void GUI90_WidgetSelectionBoxInit(int x, int y, int width, int height, GUI90_Theme theme);
bool GUI90_WidgetSelectionBoxItem(const char* text, bool is_selected);
