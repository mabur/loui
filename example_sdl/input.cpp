#include "input.hpp"

#include <assert.h>

#include <SDL2/SDL.h>

enum ButtonState {BUTTON_UP, BUTTON_CLICKED, BUTTON_DOWN, BUTTON_RELEASED};

static ButtonState s_keyboard[SDL_NUM_SCANCODES];

#define MAX_EVENTS 64
SDL_Event s_events[MAX_EVENTS];
int s_event_count = 0;

static char s_input_character = '\0';
static bool s_has_received_quit_event = false;

static ButtonState s_left_mouse_button = BUTTON_UP;
static ButtonState s_right_mouse_button = BUTTON_UP;
static ButtonState s_middle_mouse_button = BUTTON_UP;

static int s_mouse_x = 0;
static int s_mouse_y = 0;
static int s_mouse_dx = 0;
static int s_mouse_dy = 0;
static int s_mouse_wheel_dy;

static ButtonState updateButtonState(ButtonState old_state, bool is_down) {
    switch (old_state) {
        case BUTTON_UP: return is_down ? BUTTON_CLICKED : BUTTON_UP;
        case BUTTON_CLICKED: return BUTTON_DOWN;
        case BUTTON_DOWN: return is_down ? BUTTON_DOWN : BUTTON_RELEASED;
        case BUTTON_RELEASED: return BUTTON_UP;
    }
    assert(false);
    return {};
}

static void registerEvents() {
    SDL_PumpEvents();
    s_event_count = SDL_PeepEvents(s_events, MAX_EVENTS, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
    s_input_character = '\0';
    s_mouse_wheel_dy = 0;
    for (auto i = 0; i < s_event_count; ++i) {
        auto event = s_events[i];
        if (event.type == SDL_TEXTINPUT) {
            s_input_character = event.text.text[0];
        }
        if (event.type == SDL_MOUSEWHEEL) {
            s_mouse_wheel_dy = event.wheel.y;
        }
        if (event.type == SDL_QUIT) {
            s_has_received_quit_event = true;
        }
    }
    auto event = SDL_Event();
    while (SDL_PollEvent(&event)) {
    }
}

static void registerFrameKeyboardInput() {
    static bool first_time = true;
    if (first_time) {
        first_time = false;
        for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
            s_keyboard[i] = BUTTON_UP;
        }
    }
    const Uint8* sdl_keyboard = SDL_GetKeyboardState(NULL);
    for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
        s_keyboard[i] = updateButtonState(s_keyboard[i], sdl_keyboard[i]);
    }
}

static void registerFrameMouseInput(SDL_Renderer* renderer) {
    int x_window;
    int y_window;
    float x_logical;
    float y_logical;
    Uint32 mouse;
    mouse = SDL_GetRelativeMouseState(&s_mouse_dx, &s_mouse_dy);
    mouse = SDL_GetMouseState(&x_window, &y_window);
    SDL_RenderWindowToLogical(renderer, x_window, y_window, &x_logical, &y_logical);
    s_mouse_x = (int)x_logical;
    s_mouse_y = (int)y_logical;
    
    bool is_left_mouse_down = (bool)(mouse & SDL_BUTTON_LMASK);
    bool is_middle_mouse_down = (bool)(mouse & SDL_BUTTON_MMASK);
    bool is_right_mouse_down = (bool)(mouse & SDL_BUTTON_RMASK);

    s_left_mouse_button = updateButtonState(s_left_mouse_button, is_left_mouse_down);
    s_middle_mouse_button = updateButtonState(s_middle_mouse_button, is_middle_mouse_down);
    s_right_mouse_button = updateButtonState(s_right_mouse_button, is_right_mouse_down);
}

void registerFrameInput(SDL_Renderer* renderer) {
    registerEvents();
    registerFrameKeyboardInput();
    registerFrameMouseInput(renderer);
}

bool hasReceivedQuitEvent() {
    return s_has_received_quit_event;
}

SDL_Point getAbsoluteMousePosition() {return {s_mouse_x, s_mouse_y};}
SDL_Point getRelativeMousePosition() {return {s_mouse_dx, s_mouse_dy};}
int getMouseWheelY() {return s_mouse_wheel_dy;}

char getInputCharacter() {return s_input_character;}

bool isKeyUp(SDL_Scancode key) {return s_keyboard[key] == BUTTON_UP;}
bool isKeyClicked(SDL_Scancode key) {return s_keyboard[key] == BUTTON_CLICKED;}
bool isKeyDown(SDL_Scancode key) {return s_keyboard[key] == BUTTON_DOWN;}
bool isKeyReleased(SDL_Scancode key) {return s_keyboard[key] == BUTTON_RELEASED;}

bool isLeftMouseButtonUp() {return s_left_mouse_button == BUTTON_UP;}
bool isLeftMouseButtonClicked() {return s_left_mouse_button == BUTTON_CLICKED;}
bool isLeftMouseButtonDown() {return s_left_mouse_button == BUTTON_DOWN;}
bool isLeftMouseButtonReleased() {return s_left_mouse_button == BUTTON_RELEASED;}
bool isMiddleMouseButtonUp() {return s_middle_mouse_button == BUTTON_UP;}
bool isMiddleMouseButtonClicked() {return s_middle_mouse_button == BUTTON_CLICKED;}
bool isMiddleMouseButtonDown() {return s_middle_mouse_button == BUTTON_DOWN;}
bool isMiddleMouseButtonReleased() {return s_middle_mouse_button == BUTTON_RELEASED;}
bool isRightMouseButtonUp() {return s_right_mouse_button == BUTTON_UP;}
bool isRightMouseButtonClicked() {return s_right_mouse_button == BUTTON_CLICKED;}
bool isRightMouseButtonDown() {return s_right_mouse_button == BUTTON_DOWN;}
bool isRightMouseButtonReleased() {return s_right_mouse_button == BUTTON_RELEASED;}
