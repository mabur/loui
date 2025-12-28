#include "loui_image.h"

#include "draw.h"
#include "rectangle.h"
#include "state.h"

LouiImage loui_update_image(LouiImage widget) {
    drawImage(s_loui.screen, widget.x, widget.y, widget.width, widget.height, widget.data);
    auto rectangle = (LouiRectangle){
        .x=widget.x, .y=widget.y, .width=widget.width, .height=widget.height,
    };
    widget.is_clicked = isLeftMouseButtonReleasedInside(rectangle);
    return widget;
}
