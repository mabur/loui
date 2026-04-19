#include "loui_radio_buttons.h"

#include "loui_radio_button.h"

LouiRadioButtons loui_update_radio_buttons(LouiRadioButtons widget) {
    auto x = widget.x;
    auto y = widget.y;
    widget.width = 0;
    widget.height = 0;  
    for (auto i = 0; i < widget.count; ++i) {
        auto is_selected = widget.selected_index == i;
        auto text = widget.labels[i];
        auto radio_button = (LouiRadioButton){.x=x, .y=y, .text=text, .is_selected=is_selected};
        radio_button = loui_update_radio_button(radio_button);
        if (radio_button.is_clicked) {
            widget.selected_index = i;
        }
        if (radio_button.width > widget.width) {
            widget.width = radio_button.width;
        }
        widget.height += radio_button.height;
        y += radio_button.height;
    }
    return widget;
}
