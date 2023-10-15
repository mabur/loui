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
} Colors;

extern const Colors GUI90_COLORS_YELLOW;
extern const Colors GUI90_COLORS_GRAY;
extern const Colors GUI90_COLORS_LEATHER;

void GUI90_Init(int width, int height);
void GUI90_SetMouseState(int x, int y, bool is_left_mouse_button_down);
const Color* GUI90_GetPixelData();

void GUI90_WidgetBackground(Colors colors);
bool GUI90_WidgetLabel(int x, int y, const char* text, Colors colors);
bool GUI90_WidgetButton(int x, int y, const char* text, Colors colors);
int GUI90_WidgetIntSetting(int x, int y, const char* text, int value, int min_value, int max_value, Colors label_colors, Colors button_colors);

void GUI90_WidgetSelectionBoxInit(int x, int y, int width, int height, Colors colors);
bool GUI90_WidgetSelectionBoxItem(const char* text, bool is_selected);
