#define SDL_MAIN_HANDLED

#include "gui.hpp"
#include "sdl_wrappers.hpp"

int main(int, char**) {
    const auto WINDOW_TITLE = "GUI90";
    const auto WIDTH = 320;
    const auto HEIGHT = 200;
    
    auto sdl = Sdl(WINDOW_TITLE, WIDTH, HEIGHT);
    GUI90_Init(WIDTH, HEIGHT);
    sdl.setMouseModeAbsolute();
    
    while (sdl.noQuitMessage()) {
        const auto input = sdl.getInput();
        if (input.escape_button == ButtonState::CLICKED) {
            break;
        }
        GUI90_SetMouseState(input.mouse_x, input.mouse_y, input.left_mouse_button);
        GUI90_WidgetBackground(GUI90_LEATHER_SHADES);
        if (GUI90_WidgetLabel(128, 64, "Play", GUI90_LEATHER_SHADES)) {
            break;
        }
        if (GUI90_WidgetButton(128, 96, "Build", GUI90_GRAY_SHADES)) {
            break;
        }
        if (GUI90_WidgetButton(128, 128, "Exit", GUI90_GRAY_SHADES)) {
            break;
        }
        static int setting = 0;
        GUI90_WidgetIntSetting(128, 160, "Setting", &setting, GUI90_LEATHER_SHADES, GUI90_GRAY_SHADES);
        sdl.draw(GUI90_GetPixelData());
    }
    return 0;
}
