#define SDL_MAIN_HANDLED

#include "gui.hpp"
#include "sdl_wrappers.hpp"

int main(int, char**) {
    const auto WINDOW_TITLE = "GUI90";
    const auto WIDTH = 320;
    const auto HEIGHT = 200;
    
    auto sdl = Sdl(WINDOW_TITLE, WIDTH, HEIGHT);
    init(WIDTH, HEIGHT);
    sdl.setMouseModeAbsolute();
    
    while (sdl.noQuitMessage()) {
        const auto input = sdl.getInput();
        if (input.escape_button == ButtonState::CLICKED) {
            break;
        }
        setMouseState(input.mouse_x, input.mouse_y, input.left_mouse_button);
        guiBackground(LEATHER_SHADES);
        if (guiLabel(128, 64, "Play", LEATHER_SHADES)) {
            break;
        }
        if (guiButton(128, 96, "Build", GRAY_SHADES)) {
            break;
        }
        if (guiButton(128, 128, "Exit", GRAY_SHADES)) {
            break;
        }
        static int setting = 0;
        guiIntSetting(128, 160, "Setting", &setting, LEATHER_SHADES, GRAY_SHADES);
        sdl.draw(getPixelData());
    }
    return 0;
}
