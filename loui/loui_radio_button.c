#include "loui_radio_button.h"

#include "loui_label.h"
#include "rectangle.h"
#include "state.h"

LouiRadioButton loui_update_radio_button(LouiRadioButton widget) {
    for (int yi = 0; yi < 16; ++yi) {
        for (int xi = 0; xi < 16; ++xi) {
            double dx = xi - 7.5;
            double dy = yi - 7.5;
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
            if (r2 < 2.0 * 2.0 && widget.is_selected) {
                color = getTheme().recess_text_selected;
            }
            drawPoint(s_loui.screen, widget.x + xi - 2, widget.y + yi, color);
        }
    }
    auto left_rectangle = (LouiRectangle){widget.x, widget.y, 16 + BUTTON_TEXT_PADDING, 16};
    auto label = (LouiLabel){
        .x=widget.x + 2 * LOUI_BLOCK,
        .y=widget.y + BUTTON_TEXT_PADDING,
        .text=widget.text
    };
    auto label_result = loui_update_label(label);
    widget.width = label_result.width + 3 * LOUI_BLOCK;
    widget.height = 2 * LOUI_BLOCK;
    widget.is_clicked = label_result.is_clicked || isLeftMouseButtonReleasedInside(left_rectangle);
    return widget;
}
