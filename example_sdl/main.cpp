#define SDL_MAIN_HANDLED

#include <gui90/gui.hpp>

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
        if (input.escape_button == BUTTON_CLICKED) {
            break;
        }
        GUI90_SetMouseState(input.mouse_x, input.mouse_y, input.isLeftMouseButtonDown());
        GUI90_WidgetBackground(GUI90_THEME_LEATHER);
        auto x = 16;
        auto y = 16;
        if (GUI90_WidgetLabel(x, y, "Label", GUI90_THEME_LEATHER)) {
        }
        y += 16;
        if (GUI90_WidgetButton(x, y, "Button", GUI90_THEME_GRAY)) {
        }
        y += 16;
        static int setting = 0;
        setting = GUI90_WidgetIntSetting(x, y, "Setting", setting, 0, 10, GUI90_THEME_LEATHER, GUI90_THEME_GRAY);
        y += 32;
        static auto selection = 0;
        static const auto N = 4;
        const char* options[N] = {
            "Selection A",
            "Selection B",
            "Selection C",
            "Selection D",
        };
        auto width = 8 * 13;
        auto height = 8 * (N + 2);
        GUI90_WidgetSelectionBoxInit(x, y, width, height, GUI90_THEME_LEATHER);
        for (auto i = 0; i < N; ++i) {
            if (GUI90_WidgetSelectionBoxItem(options[i], selection == i)) {
                selection = i;
            }
        }
        y += height;
        
        sdl.draw(GUI90_GetPixelData());
    }
    return 0;
}
