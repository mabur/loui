#pragma once

#include "color.hpp"

enum class ButtonState {UP, CLICKED, DOWN, RELEASED};

void init(int width, int height);
void setMouseState(int x, int y, ButtonState left_mouse_button);
const Color* getPixelData();

bool guiBackground(ColorShades shades);
bool guiLabel(int x, int y, const char* text, ColorShades shades);
bool guiButton(int x, int y, const char* text, ColorShades shades);
void guiIntSetting(int x, int y, const char* text, int* value, ColorShades label_shades, ColorShades button_shades);
