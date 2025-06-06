#include "button.h"

ButtonState updateButtonState(ButtonState old_state, bool is_down) {
    switch (old_state) {
    case BUTTON_UP: return is_down ? BUTTON_CLICKED : BUTTON_UP;
    case BUTTON_CLICKED: return BUTTON_DOWN;
    case BUTTON_DOWN: return is_down ? BUTTON_DOWN : BUTTON_RELEASED;
    case BUTTON_RELEASED: return BUTTON_UP;
    default: return (ButtonState){};
    }
}

static bool is_key_repeating(int frame_tick) {
    auto INITIAL_REPEAT_TIME = 30;
    auto REPEAT_TIME = 3;
    if (frame_tick < INITIAL_REPEAT_TIME) {
        return false;
    }
    if (frame_tick == INITIAL_REPEAT_TIME) {
        return true;
    }
    return frame_tick % REPEAT_TIME == 0;
}

RepeatingButtonState updateRepeatingButtonState(RepeatingButtonState old_state, bool is_down) {
    auto button = old_state;
    if (button.state == BUTTON_UP) {
        button.frame_tick = 0;
    }
    button.state = updateButtonState(button.state, is_down);
    if (button.state == BUTTON_DOWN) {
        button.frame_tick++;
        if (is_key_repeating(old_state.frame_tick)) {
            button.state = BUTTON_CLICKED;
        }
    }
    return button;
}

bool isClicked(RepeatingButtonState button) {
    return button.state == BUTTON_CLICKED;
}
