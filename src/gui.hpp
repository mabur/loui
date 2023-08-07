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

struct Gui90;

Gui90* GUI90_Init(int width, int height);
void GUI90_Destroy(Gui90* gui);
void GUI90_SetMouseState(Gui90* gui, int x, int y, bool is_left_mouse_button_down);
const Color* GUI90_GetPixelData(const Gui90* gui);

void GUI90_WidgetBackground(Gui90* gui, Colors colors);
bool GUI90_WidgetLabel(Gui90* gui, int x, int y, const char* text, Colors colors);
bool GUI90_WidgetButton(Gui90* gui, int x, int y, const char* text, Colors colors);
int GUI90_WidgetIntSetting(Gui90* gui, int x, int y, const char* text, int value, int min_value, int max_value, Colors label_colors, Colors button_colors);

void GUI90_WidgetSelectionBoxInit(Gui90* gui, int x, int y, int width, int height, Colors colors);
bool GUI90_WidgetSelectionBoxItem(Gui90* gui, const char* text, bool is_selected);
