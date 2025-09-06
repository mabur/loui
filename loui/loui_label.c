#include "loui_label.h"

#include "draw.h"
#include "rectangle.h"
#include "state.h"

LouiLabel loui_update_label(LouiLabel widget) {
    drawString(s_loui.screen, widget.text, widget.x, widget.y, getTheme().text);
    auto rectangle = textRectangle(widget.x, widget.y, widget.text);
    widget.width = rectangle.width;
    widget.height = rectangle.height;
    widget.is_clicked = isLeftMouseButtonReleasedInside(rectangle);
    return widget;
}
