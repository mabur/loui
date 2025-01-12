#pragma once

#include <stdint.h>

typedef uint32_t GUI90_Color;

typedef struct {
    GUI90_Color background;
    GUI90_Color text;
    GUI90_Color button_background;
    GUI90_Color button_text;
    GUI90_Color button_border;
    GUI90_Color button_bevel_dark;
    GUI90_Color button_bevel_light;
    GUI90_Color recess_background;
    GUI90_Color recess_text;
    GUI90_Color recess_text_selected;
    GUI90_Color recess_bevel_dark;
    GUI90_Color recess_bevel_light;
} GUI90_Theme;

extern const GUI90_Theme GUI90_THEME_YELLOW;
extern const GUI90_Theme GUI90_THEME_GRAY;
extern const GUI90_Theme GUI90_THEME_LEATHER;
extern const GUI90_Theme GUI90_THEME_SOLARIZE_LIGHT;

void GUI90_Init(int width, int height);
void GUI90_SetMouseState(int x, int y, bool is_left_mouse_button_down);
const GUI90_Color* GUI90_GetPixelData();

void GUI90_WidgetBackground(GUI90_Theme theme);
bool GUI90_WidgetLabel(int x, int y, const char* text, GUI90_Theme theme);
bool GUI90_WidgetButton(int x, int y, const char* text, GUI90_Theme theme);
int GUI90_WidgetIntSetting(int x, int y, const char* text, int value, int min_value, int max_value, GUI90_Theme theme);

void GUI90_WidgetSelectionBoxInit(int x, int y, int width, int height, GUI90_Theme theme);
bool GUI90_WidgetSelectionBoxItem(const char* text, bool is_selected);
