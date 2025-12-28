#pragma once

#include "color.h"

typedef struct MultiLineCaret MultiLineCaret;
typedef struct LouiRectangle LouiRectangle;

typedef struct LouiScreen {
    LouiColor* data;
    int width;
    int height;
    int count;
} LouiScreen;

void drawPoint(LouiScreen screen, int x, int y, LouiColor color);
void drawImage(LouiScreen screen, int x, int y, int width, int height, LouiColor* data);
void drawCheckers(LouiScreen screen, LouiRectangle rectangle, LouiColor light, LouiColor dark);
void drawRectangle(LouiScreen screen, LouiRectangle rectangle, LouiColor color);
void drawRectangleCorners(LouiScreen screen, LouiRectangle rectangle, LouiColor color);
void drawRoundedRectangleOutline(LouiScreen screen, LouiRectangle rectangle, LouiColor color_top_left, LouiColor color_bottom_right);
void drawLineHorizontal(LouiScreen screen, int x, int y, int width, LouiColor color);
void drawLineVertical(LouiScreen screen, int x, int y, int height, LouiColor color);
void drawCharacter(LouiScreen screen, char character, size_t x_start, size_t y_start, LouiColor color);
void drawString(LouiScreen screen, const char* s, size_t x, size_t y, LouiColor color);

void drawCaret(LouiScreen screen, size_t x_start, size_t y_start, LouiColor color);
void drawMultiLineCaret(
    LouiScreen screen,
    size_t x_start,
    size_t y_start,
    LouiColor color,
    MultiLineCaret caret,
    MultiLineCaret draw_caret
);
void drawMultiLineString(
    LouiScreen screen,
    const char* s,
    size_t x,
    size_t y,
    LouiColor color,
    LouiColor selection_background_color,
    int max_lines,
    int max_columns,
    MultiLineCaret caret,
    MultiLineCaret selection_anchor,
    MultiLineCaret draw_caret
);

LouiRectangle textRectangle(int x, int y, const char* text);
