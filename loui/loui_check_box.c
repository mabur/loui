#include "loui_check_box.h"

#include "draw.h"
#include "loui_label.h"
#include "loui_sunken_frame.h"
#include "rectangle.h"
#include "state.h"

LouiCheckBox loui_update_check_box(LouiCheckBox widget) {
    auto x = widget.x;
    auto y = widget.y;
    auto box_size = 10;
    auto frame = (LouiSunkenFrame){
        .x=x,// + (16 - box_size) / 2,
        .y=y + (16 - box_size) / 2,
        .width=box_size,
        .height=box_size,
    };
    frame = loui_update_sunken_frame(frame);

    int check[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 0, 0,
        0, 0, 1, 0, 0, 1, 0, 0,
        0, 0, 0, 1, 1, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
    };
    if (widget.is_selected) {
        for (auto dy = 0; dy < 8; ++dy) {
            for (auto dx = 0; dx < 8; ++dx) {
                if (check[dy * 8 + dx]) {
                    drawPoint(s_loui.screen, x + 1 + dx, y + 4 + dy, getTheme().recess_text_selected);
                }
            }
        }
    }

    auto left_rectangle = (Rectangle){widget.x, widget.y, 16 + BUTTON_TEXT_PADDING, 16};
    auto label = (LouiLabel){
        .x=widget.x + 2 * LOUI_BLOCK,
        .y=widget.y + BUTTON_TEXT_PADDING,
        .text=widget.text
    };
    auto label_result = loui_update_label(label);
    widget.width = label_result.width + 3 * LOUI_BLOCK;
    widget.height = 2 * LOUI_BLOCK;
    widget.is_clicked = label_result.is_clicked || isLeftMouseButtonReleasedInside(left_rectangle);
    if (widget.is_clicked) {
        widget.is_selected = !widget.is_selected;
    }
    return widget;
}
