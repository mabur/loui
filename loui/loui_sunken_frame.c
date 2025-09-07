#include "loui_sunken_frame.h"

#include "draw.h"
#include "rectangle.h"
#include "state.h"

LouiSunkenFrame loui_update_sunken_frame(LouiSunkenFrame widget) {
    auto x = widget.x;
    auto y = widget.y;
    auto width = widget.width;
    auto height = widget.height;
    auto rectangle = (Rectangle){x, y, width, height};
    auto inner_rectangle = shrinkRectangle(rectangle);
    drawRectangle(s_loui.screen, inner_rectangle, getTheme().recess_background);
    drawRoundedRectangleOutline(
        s_loui.screen, rectangle, getTheme().recess_bevel_dark, getTheme().recess_bevel_light
    );
    widget.is_clicked = isLeftMouseButtonReleasedInside(inner_rectangle);
    return widget;
}
