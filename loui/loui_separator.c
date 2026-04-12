#include "loui_separator.h"

#include "draw.h"
#include "rectangle.h"
#include "state.h"

LouiSeparator loui_update_separator(LouiSeparator widget) {
    widget.height = 8;
    for (auto x = widget.x; x < widget.x + widget.width; x += 2) {
        drawPoint(s_loui.screen, x, widget.y + 3, getTheme().recess_bevel_dark);    
    }
    return widget;
}
