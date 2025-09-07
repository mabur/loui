#include "loui_window.h"

#include "draw.h"
#include "rectangle.h"
#include "state.h"

LouiWindow loui_update_window(LouiWindow widget) {
    auto x = widget.x;
    auto y = widget.y;
    auto width = widget.width;
    auto height = widget.height;
    auto rectangle = (Rectangle){x, y, width, height};
    auto inner_rectangle = shrinkRectangle(rectangle);
    auto innermost_rectangle = shrinkRectangle(inner_rectangle);

    auto border_color = getTheme().button_border;
    auto background_color = getTheme().background;
    auto light_bevel_color = getTheme().recess_bevel_light;
    auto dark_bevel_color = getTheme().recess_bevel_dark;

    drawRoundedRectangleOutline(s_loui.screen, rectangle, border_color, border_color);
    drawRoundedRectangleOutline(s_loui.screen, inner_rectangle, light_bevel_color, dark_bevel_color);
    drawRectangleCorners(s_loui.screen, inner_rectangle, border_color);
    drawRectangle(s_loui.screen, innermost_rectangle, background_color);

    widget.is_clicked = isLeftMouseButtonReleasedInside(innermost_rectangle);
    return widget;
}
