#include "loui_knob.h"

#include <math.h>

#include "rectangle.h"
#include "state.h"

LouiKnob loui_update_knob(LouiKnob widget) {
    widget.width = 16;
    widget.height = 16;
    auto rectangle = (LouiRectangle){widget.x, widget.y, widget.width, widget.height};
    widget.is_clicked = isLeftMouseButtonReleasedInside(rectangle);
    if (isLeftMouseButtonDownInside(rectangle)) {
        auto dx = s_loui.mouse_x - (widget.x + 7.5);
        auto dy = s_loui.mouse_y - (widget.y + 7.5);
        widget.angle = atan2(dy, dx);
    }
    for (int yi = 0; yi < 16; ++yi) {
        for (int xi = 0; xi < 16; ++xi) {
            double dx = xi - 7.5;
            double dy = yi - 7.5;
            double angle = atan2(dy, dx);
            double da = fabs(angle - widget.angle);
            double r2 = dx * dx + dy * dy;
            auto color = getTheme().background;
            if (r2 < 6.0 * 6.0 && dx + dy < 0.0) {
                color = getTheme().recess_bevel_dark;
            }
            if (r2 < 6.0 * 6.0 && dx + dy > 0.0) {
                color = getTheme().recess_bevel_light;
            }
            if (r2 < 5.0 * 5.0) {
                color = getTheme().recess_background;
            }
            if (r2 < 5.0 * 5.0 && da < 3.14 * 0.07) {
                color = getTheme().recess_text_selected;
            }
            if (r2 < 2.0 * 2.0 && dx + dy < 0.0) {
                color = getTheme().recess_bevel_light;
            }
            if (r2 < 2.0 * 2.0 && dx + dy > 0.0) {
                color = getTheme().recess_bevel_dark;
            }
            if (r2 < 1.0 * 1.0) {
                color = getTheme().background;
            }
            drawPoint(s_loui.screen, widget.x + xi, widget.y + yi, color);
        }
    }
    return widget;
}
