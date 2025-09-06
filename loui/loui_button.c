#include "loui_button.h"

#include <string.h>

#include "rectangle.h"
#include "state.h"

void drawButton(Rectangle rectangle, const char* text) {
    // Inside the bevels:
    auto inner_rectangle = shrinkRectangle(rectangle);
    auto innermost_rectangle = shrinkRectangle(inner_rectangle);
    auto text_x = rectangle.x - 1 + BUTTON_TEXT_PADDING;
    auto text_y = rectangle.y - 1 + BUTTON_TEXT_PADDING;
    auto theme = getTheme();
    auto screen = s_loui.screen;

    auto background_color = theme.button_background;
    auto border_color = theme.button_border;
    auto light_bevel_color = theme.button_bevel_light;
    auto dark_bevel_color = theme.button_bevel_dark;

    if (isLeftMouseButtonDownInside(rectangle)) {
        ++text_y;
        light_bevel_color = background_color;
        dark_bevel_color = background_color;
    }
    drawRectangle(screen, rectangle, border_color);
    drawRectangle(screen, innermost_rectangle, background_color);
    drawRoundedRectangleOutline(
        screen, inner_rectangle, light_bevel_color, dark_bevel_color
    );
    drawString(screen, text, text_x, text_y, theme.button_text);
}

static
LouiButton loui_update_button_bevel(LouiButton widget) {
    auto rectangle = (Rectangle){};
    rectangle.x = widget.x + 1;
    rectangle.y = widget.y + 1;
    rectangle.width = 8 * (int)(strlen(widget.text)) + 6;
    rectangle.height = 8 + 6;

    drawButton(rectangle, widget.text);

    widget.width = rectangle.width + 2;
    widget.height = rectangle.height + 2;
    widget.is_clicked = isLeftMouseButtonReleasedInside(rectangle);
    return widget;
}

static
LouiButton loui_update_button_cloud(LouiButton widget) {
    auto x = widget.x;
    auto y = widget.y;
    x += 1;
    y += 1;
    auto rectangle = (Rectangle){};
    rectangle.x = x;
    rectangle.y = y;
    rectangle.width = 8 * (int)(strlen(widget.text)) + 6;
    rectangle.height = 8 + 6 - 1;

    if (isLeftMouseButtonDownInside(rectangle)) {
        rectangle.y += 1;
    }

    auto inner_rectangle = shrinkRectangle(rectangle);
    auto text_x = x + BUTTON_TEXT_PADDING - 1;
    auto text_y = y + BUTTON_TEXT_PADDING - 2;
    auto theme = getTheme();
    if (isLeftMouseButtonDownInside(rectangle)) {
        ++text_y;
    }

    // Shadow:
    drawPoint(s_loui.screen, x, y + rectangle.height - 1, theme.button_border);
    drawPoint(s_loui.screen, x + rectangle.width - 1, y + rectangle.height - 1, theme.button_border);
    drawLineHorizontal(s_loui.screen, x + 1, y + rectangle.height, rectangle.width - 2, theme.button_border);

    //drawRectangle(rectangle, getTheme().button_border);
    drawRectangle(s_loui.screen, inner_rectangle, theme.button_background);

    // Rounded corners:
    drawLineHorizontal(s_loui.screen, rectangle.x + 1, rectangle.y, rectangle.width - 2, theme.button_border);
    drawLineHorizontal(s_loui.screen, rectangle.x + 1, rectangle.y + rectangle.height - 1, rectangle.width - 2, theme.button_border);
    drawLineVertical(s_loui.screen, rectangle.x, rectangle.y + 1, rectangle.height - 2, theme.button_border);
    drawLineVertical(s_loui.screen, rectangle.x + rectangle.width - 1, rectangle.y + 1, rectangle.height - 2, theme.button_border);
    drawPoint(s_loui.screen, rectangle.x + 1, rectangle.y + 1, theme.button_border);
    drawPoint(s_loui.screen, rectangle.x + 1, rectangle.y + rectangle.height - 2, theme.button_border);
    drawPoint(s_loui.screen, rectangle.x + + rectangle.width - 2, rectangle.y + 1, theme.button_border);
    drawPoint(s_loui.screen, rectangle.x + + rectangle.width - 2, rectangle.y + rectangle.height - 2, theme.button_border);

    drawPoint(s_loui.screen, rectangle.x + 2, rectangle.y + 1, theme.button_bevel_dark);
    drawPoint(s_loui.screen, rectangle.x + 1, rectangle.y + 2, theme.button_bevel_dark);
    drawPoint(s_loui.screen, rectangle.x + 1, rectangle.y + rectangle.height - 3, theme.button_bevel_dark);
    drawPoint(s_loui.screen, rectangle.x + 2, rectangle.y + rectangle.height - 2, theme.button_bevel_dark);
    drawPoint(s_loui.screen, rectangle.x + rectangle.width - 3, rectangle.y + 1, theme.button_bevel_dark);
    drawPoint(s_loui.screen, rectangle.x + rectangle.width - 2, rectangle.y + 2, theme.button_bevel_dark);
    drawPoint(s_loui.screen, rectangle.x + rectangle.width - 3, rectangle.y + rectangle.height - 2, theme.button_bevel_dark);
    drawPoint(s_loui.screen, rectangle.x + rectangle.width - 2, rectangle.y + rectangle.height - 3, theme.button_bevel_dark);

    drawString(s_loui.screen, widget.text, text_x, text_y, theme.button_text);
    widget.width = rectangle.width + 2;
    widget.height = 2 * LOUI_BLOCK;
    widget.is_clicked = isLeftMouseButtonReleasedInside(rectangle);
    return widget;
}

LouiButton loui_update_button(LouiButton widget) {
    switch (getTheme().button_type) {
        case BUTTON_TYPE_BEVEL: return loui_update_button_bevel(widget);
        case BUTTON_TYPE_CLOUD: return loui_update_button_cloud(widget);
        default: return loui_update_button_bevel(widget);
    }
}
