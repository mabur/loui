#pragma once

typedef enum ButtonState {
    BUTTON_UP, BUTTON_CLICKED, BUTTON_DOWN, BUTTON_RELEASED
} ButtonState;

typedef struct RepeatingButtonState {
    ButtonState state;
    int frame_tick;
} RepeatingButtonState;

ButtonState updateButtonState(ButtonState old_state, bool is_down);
RepeatingButtonState updateRepeatingButtonState(RepeatingButtonState old_state, bool is_down);
bool isClicked(RepeatingButtonState button);
