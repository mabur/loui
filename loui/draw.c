#include "draw.h"

#include <string.h>

#include "multi_line_caret.h"
#include "rectangle.h"
#include "font.h"

void drawPoint(LouiScreen screen, int x, int y, LouiColor color) {
    screen.data[y * screen.width + x] = color;
}

void drawImage(LouiScreen screen, int x, int y, int width, int height, LouiColor* data) {
    auto i_source = 0;
    for (auto y_source = 0; y_source < height; ++y_source) {
        for (auto x_source = 0; x_source < width; ++x_source) {
            auto i_target = (y + y_source) * screen.width + x + x_source;
            screen.data[i_target] = data[i_source];
            i_source++;
        }
    }
}

void drawCheckers(LouiScreen screen, LouiRectangle rectangle, LouiColor light, LouiColor dark) {
    for (auto dy = 0; dy < rectangle.height; ++dy) {
        for (auto dx = 0; dx < rectangle.width; ++dx) {
            auto x = rectangle.x + dx;
            auto y = rectangle.y + dy;
            auto color = (x + y) % 2 ? light : dark;
            drawPoint(screen, x, y, color);
        }
    }
}

void drawRectangle(LouiScreen screen, LouiRectangle rectangle, LouiColor color) {
    for (auto dy = 0; dy < rectangle.height; ++dy) {
        for (auto dx = 0; dx < rectangle.width; ++dx) {
            drawPoint(screen, rectangle.x + dx, rectangle.y + dy, color);
        }
    }
}

void drawRectangleCorners(LouiScreen screen, LouiRectangle rectangle, LouiColor color) {
    auto x = rectangle.x;
    auto y = rectangle.y;
    auto width = rectangle.width;
    auto height = rectangle.height;
    drawPoint(screen, x, y, color);
    drawPoint(screen, x + width - 1, y, color);
    drawPoint(screen, x, y + height - 1, color);
    drawPoint(screen, x + width - 1, y + height - 1, color);
}

void drawRoundedRectangleOutline(LouiScreen screen, LouiRectangle rectangle, LouiColor color_top_left, LouiColor color_bottom_right) {
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
    auto r = (LouiRectangle){};
    r.x = x;
    r.y = y;
    r.width = width;
    r.height = 1;
    drawRectangle(screen, r, color);
}

void drawLineVertical(LouiScreen screen, int x, int y, int height, LouiColor color) {
    auto r = (LouiRectangle){};
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

void drawCaret(
    LouiScreen screen, size_t x_start, size_t y_start, LouiColor color) {
    for (size_t y = 0; y < 8; ++y) {
        screen.data[(y_start + y) * screen.width + x_start] = color;
    }
}

void drawMultiLineCaret(
    LouiScreen screen,
    size_t x_start,
    size_t y_start,
    LouiColor color,
    MultiLineCaret caret,
    MultiLineCaret draw_caret
) {
    auto draw_caret_line = caret.line - draw_caret.line;
    auto draw_caret_column = caret.column - draw_caret.column;
    auto caret_x = x_start + draw_caret_column * 8;
    auto caret_y = y_start + draw_caret_line * 8;
    drawCaret(screen, caret_x, caret_y, color);
}

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
) {
    auto selection_begin = minMultiLineCaret(caret, selection_anchor);
    auto selection_end = maxMultiLineCaret(caret, selection_anchor);

    auto current = (MultiLineCaret){};
    for (; *s; ++s) {
        auto draw_line = current.line - draw_caret.line;
        auto draw_column = current.column - draw_caret.column;
        auto draw_x = x + draw_column * 8;
        auto draw_y = y + draw_line * 8;
        if (0 <= draw_line && draw_line < max_lines &&
            0 <= draw_column && draw_column < max_columns
        ) {
            if (isBetween(selection_begin, current, selection_end)) {
                auto rectangle = (LouiRectangle){.x=draw_x, .y=draw_y, .width=8, .height=8};
                drawRectangle(screen, rectangle, selection_background_color);
            }
            drawCharacter(screen, *s, draw_x, draw_y, color);
        }
        if (*s == '\n') {
            current.column = 0;
            current.line++;
        }
        else {
            current.column++;
        }
    }
}

LouiRectangle textRectangle(int x, int y, const char* text) {
    return (LouiRectangle){
        .x = x,
        .y = y,
        .width = 8 * (int)(strlen(text)),
        .height = 8,
    };
}
