#pragma once

#include "color.hpp"

enum class ButtonState {UP, CLICKED, DOWN, RELEASED};

void init(int width, int height);
void setMouseState(int x, int y, ButtonState left_mouse_button);
const Color* getPixelData();

bool guiBackground(Color color);
bool guiLabel(int x, int y, const char* text, Color text_color = WHITE);
bool guiButton(int x, int y, const char* text, ColorShades shades);
void guiIntSetting(int x, int y, const char* text, ColorShades shades, int* value);
