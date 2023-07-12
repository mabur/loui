#pragma once

#include "color.hpp"

enum class ButtonState {UP, CLICKED, DOWN, RELEASED};

void GUI90_Init(int width, int height);
void GUI90_SetMouseState(int x, int y, ButtonState left_mouse_button);
const Color* GUI90_GetPixelData();

void GUI90_WidgetBackground(ColorShades shades);
bool GUI90_WidgetLabel(int x, int y, const char* text, ColorShades shades);
bool GUI90_WidgetButton(int x, int y, const char* text, ColorShades shades);
void GUI90_WidgetIntSetting(int x, int y, const char* text, int* value, ColorShades label_shades, ColorShades button_shades);
