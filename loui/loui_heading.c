#include "loui_heading.h"

#include "draw.h"
#include "rectangle.h"
#include "state.h"

static void drawSpecialString(
    LouiScreen screen, const char* s, int x, int y, LouiHeadingTheme theme
) {
    for (; *s; ++s, x += 8) {
        if (theme.draw_up_left) {
            drawCharacter(screen, *s, x - 1, y - 1, theme.color_up_left);
        }
        if (theme.draw_up_right) {
            drawCharacter(screen, *s, x + 1, y - 1, theme.color_up_right);
        }
        if (theme.draw_down_left) {
            drawCharacter(screen, *s, x - 1, y + 1, theme.color_down_left);
        }
        if (theme.draw_down_right) {
            drawCharacter(screen, *s, x + 1, y + 1, theme.color_down_right);
        }

        if (theme.draw_up) {
            drawCharacter(screen, *s, x + 0, y - 1, theme.color_up);
        }
        if (theme.draw_left) {
            drawCharacter(screen, *s, x - 1, y + 0, theme.color_left);
        }
        if (theme.draw_right) {
            drawCharacter(screen, *s, x + 1, y + 0, theme.color_right);
        }
        if (theme.draw_down) {
            drawCharacter(screen, *s, x + 0, y + 1, theme.color_down);
        }

        if (theme.draw_center) {
            drawCharacter(screen, *s, x + 0, y + 0, theme.color_center);
        }
    }
}

LouiHeading loui_update_heading(LouiHeading widget) {
    drawSpecialString(s_loui.screen, widget.text, widget.x, widget.y, widget.theme);
    auto rectangle = textRectangle(widget.x, widget.y, widget.text);
    widget.width = rectangle.width;
    widget.height = rectangle.height;
    widget.is_clicked = isLeftMouseButtonReleasedInside(rectangle);
    return widget;
}
