#include "loui_selection_box_item.h"

#include "loui_label.h"
#include "state.h"

LouiSelectionBoxItem loui_update_selection_box_item(LouiSelectionBoxItem widget) {
    auto global_theme = getTheme();
    auto local_theme = getTheme();
    local_theme.text = widget.is_selected ? local_theme.recess_text_selected : local_theme.recess_text;
    loui_set_theme(local_theme);
    auto label = (LouiLabel){
        .x=s_loui.current_x,
        .y=s_loui.current_y,
        .text=widget.text
    };
    label = loui_update_label(label);
    s_loui.current_y += label.height;
    loui_set_theme(global_theme);
    widget.width = label.width;
    widget.height = label.height;
    widget.is_clicked = label.is_clicked;
    return widget;
}
