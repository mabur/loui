#define SDL_MAIN_HANDLED

#include "gui.hpp"
#include "sdl_wrappers.hpp"

int main(int, char**) {
    const auto WINDOW_TITLE = "GUI90";
    const auto WIDTH = 320;
    const auto HEIGHT = 200;
    
    auto sdl = Sdl(WINDOW_TITLE, WIDTH, HEIGHT);
    auto gui = GUI90_Init(WIDTH, HEIGHT);
    sdl.setMouseModeAbsolute();
    
    while (sdl.noQuitMessage()) {
        const auto input = sdl.getInput();
        if (input.escape_button == BUTTON_CLICKED) {
            break;
        }
        GUI90_SetMouseState(gui, input.mouse_x, input.mouse_y, input.left_mouse_button);
        GUI90_WidgetBackground(gui, GUI90_LEATHER_SHADES);
        if (GUI90_WidgetLabel(gui, 128, 64, "Play", GUI90_LEATHER_SHADES)) {
            break;
        }
        if (GUI90_WidgetButton(gui, 128, 96, "Build", GUI90_GRAY_SHADES)) {
            break;
        }
        if (GUI90_WidgetButton(gui, 128, 128, "Exit", GUI90_GRAY_SHADES)) {
            break;
        }
        static int setting = 0;
        GUI90_WidgetIntSetting(gui, 128, 160, "Setting", &setting, GUI90_LEATHER_SHADES, GUI90_GRAY_SHADES);
        sdl.draw(GUI90_GetPixelData(gui));
    }
    GUI90_Destroy(gui);
    return 0;
}
