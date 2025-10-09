#include "state.h"

#include "rectangle.h"

LouiState s_loui;

LouiTheme getTheme() {
    return s_loui.theme;
}

void loui_set_theme(LouiTheme theme) {
    s_loui.theme = theme;
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

bool isShiftUp() {
    return s_loui.modifier_keys[LOUI_MODIFIER_KEY_SHIFT] == BUTTON_UP;
}

bool isControlDown() {
    return s_loui.modifier_keys[LOUI_MODIFIER_KEY_CONTROL] == BUTTON_DOWN;
}
