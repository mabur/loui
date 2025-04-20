#include "draw.h"

#include "text.h"

void drawPoint(LouiScreen screen, int x, int y, LouiColor color) {
    screen.data[y * screen.width + x] = color;
}

void drawRectangle(LouiScreen screen, Rectangle rectangle, LouiColor color) {
    for (auto dy = 0; dy < rectangle.height; ++dy) {
        for (auto dx = 0; dx < rectangle.width; ++dx) {
            drawPoint(screen, rectangle.x + dx, rectangle.y + dy, color);
        }
    }
}

void drawLineHorizontal(LouiScreen screen, int x, int y, int width, LouiColor color) {
    auto r = (Rectangle){};
    r.x = x;
    r.y = y;
    r.width = width;
    r.height = 1;
    drawRectangle(screen, r, color);
}

void drawLineVertical(LouiScreen screen, int x, int y, int height, LouiColor color) {
    auto r = (Rectangle){};
    r.x = x;
    r.y = y;
    r.width = 1;
    r.height = height;
    drawRectangle(screen, r, color);
}

void drawCharacter(LouiScreen screen, char character, size_t x_start, size_t y_start, LouiColor color) {
    auto W = screen.width;
    auto character_bitmap = character_bitmap8x8(character);
    for (size_t y = 0; y < 8; ++y) {
        for (size_t x = 0; x < 8; ++x) {
            if (character_bitmap[y * 8 + x]) {
                screen.data[(y_start + y) * W + x_start + x] = color;
            }
        }
    }
}

void drawString(LouiScreen screen, const char* s, size_t x, size_t y, LouiColor color) {
    for (; *s; ++s, x += 8) {
        drawCharacter(screen, *s, x, y, color);
    }
}
