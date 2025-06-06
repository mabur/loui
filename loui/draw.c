#include "draw.h"

#include "rectangle.h"
#include "text.h"

void drawPoint(LouiScreen screen, int x, int y, LouiColor color) {
    screen.data[y * screen.width + x] = color;
}

void drawCheckers(LouiScreen screen, Rectangle rectangle, LouiColor light, LouiColor dark) {
    for (auto dy = 0; dy < rectangle.height; ++dy) {
        for (auto dx = 0; dx < rectangle.width; ++dx) {
            auto x = rectangle.x + dx;
            auto y = rectangle.y + dy;
            auto color = (x + y) % 2 ? light : dark;
            drawPoint(screen, x, y, color);
        }
    }
}

void drawRectangle(LouiScreen screen, Rectangle rectangle, LouiColor color) {
    for (auto dy = 0; dy < rectangle.height; ++dy) {
        for (auto dx = 0; dx < rectangle.width; ++dx) {
            drawPoint(screen, rectangle.x + dx, rectangle.y + dy, color);
        }
    }
}

void drawRectangleCorners(LouiScreen screen, Rectangle rectangle, LouiColor color) {
    auto x = rectangle.x;
    auto y = rectangle.y;
    auto width = rectangle.width;
    auto height = rectangle.height;
    drawPoint(screen, x, y, color);
    drawPoint(screen, x + width - 1, y, color);
    drawPoint(screen, x, y + height - 1, color);
    drawPoint(screen, x + width - 1, y + height - 1, color);
}

void drawRoundedRectangleOutline(LouiScreen screen, Rectangle rectangle, LouiColor color_top_left, LouiColor color_bottom_right) {
    auto x_left = rectangle.x;
    auto y_top = rectangle.y;
    auto width = rectangle.width;
    auto height = rectangle.height;
    auto x_right = x_left + width - 1;
    auto y_bottom = y_top + height - 1;
    drawLineHorizontal(screen, x_left + 1, y_top, width - 2, color_top_left);
    drawLineHorizontal(screen, x_left + 1, y_bottom, width - 2, color_bottom_right);
    drawLineVertical(screen, x_left, y_top + 1, height - 2, color_top_left);
    drawLineVertical(screen, x_right, y_top + 1, height - 2, color_bottom_right);
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
