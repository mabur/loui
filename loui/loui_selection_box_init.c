#include "loui_selection_box_init.h"

#include "loui_sunken_frame.h"
#include "state.h"

LouiSelectionBoxInit loui_update_selection_box_init(LouiSelectionBoxInit widget) {
    s_loui.current_x = widget.x + TEXT_SIZE;
    s_loui.current_y = widget.y + TEXT_SIZE;
    auto frame = (LouiSunkenFrame){.x=widget.x, .y=widget.y, .width=widget.width, .height=widget.height};
    frame = loui_update_sunken_frame(frame);
    return widget;
}
