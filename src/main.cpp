#define SDL_MAIN_HANDLED

#include "gui.hpp"
#include "sdl_wrappers.hpp"

int main(int, char**) {
    const auto WINDOW_TITLE = "GUI90";
    const auto WIDTH = 320;
    const auto HEIGHT = 200;
    
    auto blue_shade = ColorShades{};
    blue_shade.foreground = packColorRgb(255, 255, 255);
    blue_shade.background = packColorRgb(0, 0, 255);
    blue_shade.bevel_light = packColorRgb(80, 80, 255);
    blue_shade.bevel_dark = packColorRgb(0, 0, 170);
    blue_shade.border = packColorRgb(0, 0, 0);
    
    auto sdl = Sdl(WINDOW_TITLE, WIDTH, HEIGHT);
    init(WIDTH, HEIGHT);
    sdl.setMouseModeAbsolute();

    while (sdl.noQuitMessage()) {
        const auto input = sdl.getInput();
        if (input.escape_button == ButtonState::CLICKED) {
            break;
        }
        setMouseState(input.mouse_x, input.mouse_y, input.left_mouse_button);
        guiBackground(GRAY);
        if (guiLabel(128, 64, "Play")) {
            break;
        }
        if (guiLabel(128, 96, "Build")) {
            break;
        }
        if (guiButton(128, 128, "Exit", blue_shade)) {
            break;
        }
        static int setting = 0;
        guiIntSetting(128, 160, "Setting", blue_shade, &setting);
        sdl.draw(getPixelData());
    }
    return 0;
}
