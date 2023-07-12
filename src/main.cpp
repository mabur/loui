#define SDL_MAIN_HANDLED

#include "gui.hpp"
#include "sdl_wrappers.hpp"

int main(int, char**) {
    const auto WINDOW_TITLE = "GUI90";
    const auto WIDTH = 320;
    const auto HEIGHT = 200;
    
    auto color_shades = ColorShades{};
    color_shades.foreground = packColorRgb(0, 0, 0);
    color_shades.background = packColorRgb(255, 221, 63);
    color_shades.bevel_light = packColorRgb(255, 245, 197);
    color_shades.bevel_dark = packColorRgb(207, 117, 43);
    color_shades.border = packColorRgb(0, 0, 0);
    
    auto sdl = Sdl(WINDOW_TITLE, WIDTH, HEIGHT);
    init(WIDTH, HEIGHT);
    sdl.setMouseModeAbsolute();

    while (sdl.noQuitMessage()) {
        const auto input = sdl.getInput();
        if (input.escape_button == ButtonState::CLICKED) {
            break;
        }
        setMouseState(input.mouse_x, input.mouse_y, input.left_mouse_button);
        guiBackground(packColorRgb(128, 128, 128));
        if (guiLabel(128, 64, "Play")) {
            break;
        }
        if (guiButton(128, 96, "Build", color_shades)) {
            break;
        }
        if (guiButton(128, 128, "Exit", color_shades)) {
            break;
        }
        static int setting = 0;
        guiIntSetting(128, 160, "Setting", color_shades, &setting);
        sdl.draw(getPixelData());
    }
    return 0;
}
