#include "state.h"

#include "rectangle.h"

LouiState s_loui;

const int TEXT_SIZE = 8;
const int BUTTON_TEXT_PADDING = 4;

LouiTheme getTheme() {
    return s_loui.theme;
}

bool isLeftMouseButtonDownInside(Rectangle r) {
    return s_loui.is_input_enabled && s_loui.left_mouse_button == BUTTON_DOWN &&
           r.x <= s_loui.mouse_x && s_loui.mouse_x < r.x + r.width &&
           r.y <= s_loui.mouse_y && s_loui.mouse_y < r.y + r.height;
}

bool isLeftMouseButtonReleasedInside(Rectangle r) {
    return s_loui.is_input_enabled && s_loui.left_mouse_button == BUTTON_RELEASED &&
           r.x <= s_loui.mouse_x && s_loui.mouse_x < r.x + r.width &&
           r.y <= s_loui.mouse_y && s_loui.mouse_y < r.y + r.height;
}
