#include "loui_stepper.h"

#include "loui_label.h"
#include "loui_button.h"
#include "state.h"

LouiStepper loui_update_stepper(LouiStepper widget) {
    auto x = widget.x;
    auto y = widget.y;
    auto offset = 0;
    auto label = (LouiLabel){
        .x=x + offset,
        .y=y + BUTTON_TEXT_PADDING,
        .text=widget.text
    };
    auto label_widget = loui_update_label(label);
    offset += label_widget.width;
    auto decrease_button = (LouiButton){.x=x + offset, .y=y, .text="-"};
    decrease_button = loui_update_button(decrease_button);
    offset += decrease_button.width;
    auto increase_button = (LouiButton){.x=x + offset, .y=y, .text="+"};
    increase_button = loui_update_button(increase_button);
    offset += increase_button.width;

    widget.width = offset;
    widget.height = increase_button.height;
    widget.is_clicked = increase_button.is_clicked || decrease_button.is_clicked;
    widget.is_increased = increase_button.is_clicked;
    widget.is_decreased = decrease_button.is_clicked;
    return widget;
}
