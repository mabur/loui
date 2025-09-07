#include "loui_slider.h"

#include "loui_sunken_frame.h"
#include "rectangle.h"
#include "state.h"

LouiSlider loui_update_slider(LouiSlider widget) {
    auto horizontal_padding = LOUI_BLOCK;
    widget.height = LOUI_BLOCK * 2;
    auto x = widget.x;
    auto y = widget.y;
    auto width = widget.width;
    auto height = widget.height;
    auto rectangle = (Rectangle){.x=x, .y=y, .width=width, .height=height};

    LouiSunkenFrame sunken_frame = {.x=x+horizontal_padding, .y=y+7, .width=width - 2*horizontal_padding, .height=3};
    loui_update_sunken_frame(sunken_frame);

    auto background_color = getTheme().button_background;
    auto border_color = getTheme().button_border;
    auto light_bevel_color = getTheme().button_bevel_light;
    auto dark_bevel_color = getTheme().button_bevel_dark;

    if (isLeftMouseButtonDownInside(rectangle)) {
        light_bevel_color = background_color;
        dark_bevel_color = background_color;
        widget.value = (double)(s_loui.mouse_x - x - horizontal_padding) / (width - 1 - 2 * horizontal_padding);
        widget.value = widget.value < 0.0 ? 0.0 : widget.value;
        widget.value = widget.value > 1.0 ? 1.0 : widget.value;
    }

    auto button = (Rectangle){};
    button.width = 6;
    button.height = 13;
    button.x = sunken_frame.x + widget.value * (sunken_frame.width - button.width);
    button.y = widget.y + 2;

    // Begin draw button
    auto inner_rectangle = shrinkRectangle(button);
    auto innermost_rectangle = shrinkRectangle(inner_rectangle);
    auto screen = s_loui.screen;

    drawRoundedRectangleOutline(screen, button, border_color, border_color);
    drawRectangle(screen, innermost_rectangle, background_color);
    drawLineHorizontal(screen, inner_rectangle.x, inner_rectangle.y, inner_rectangle.width, light_bevel_color);
    drawLineVertical(screen, inner_rectangle.x, inner_rectangle.y, inner_rectangle.height, light_bevel_color);
    drawLineHorizontal(screen, inner_rectangle.x, inner_rectangle.y + inner_rectangle.height - 1, inner_rectangle.width, dark_bevel_color);
    drawLineVertical(screen, inner_rectangle.x + inner_rectangle.width - 1, inner_rectangle.y, inner_rectangle.height, dark_bevel_color);
    // End draw button

    widget.is_down = isLeftMouseButtonDownInside(rectangle);
    return widget;
}
