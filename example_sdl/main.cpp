#define SDL_MAIN_HANDLED

#include <assert.h>

#include <gui90/gui.hpp>

#include "sdl_wrappers.hpp"

enum GuiThemeIndex {
    GRAY_THEME_INDEX,
    WARM_GRAY_THEME_INDEX,
    SOLARIZE_LIGHT_THEME_INDEX,
    YELLOW_THEME_INDEX,
    LEATHER_THEME_INDEX,
    THEME_COUNT,
};

GUI90_Theme themeSettings(GuiThemeIndex theme) {
    switch (theme) {
        case YELLOW_THEME_INDEX: return GUI90_THEME_YELLOW;
        case GRAY_THEME_INDEX: return GUI90_THEME_GRAY;
        case WARM_GRAY_THEME_INDEX: return GUI90_THEME_WARM_GRAY;
        case LEATHER_THEME_INDEX: return GUI90_THEME_LEATHER;
        case SOLARIZE_LIGHT_THEME_INDEX: return GUI90_THEME_SOLARIZE_LIGHT;
    }
    assert(false and "Unknown theme");
}

const char* themeDescription(GuiThemeIndex theme) {
    switch (theme) {
        case YELLOW_THEME_INDEX: return "Yellow";
        case GRAY_THEME_INDEX: return "Gray";
        case WARM_GRAY_THEME_INDEX: return "Warm Gray";
        case LEATHER_THEME_INDEX: return "Leather";
        case SOLARIZE_LIGHT_THEME_INDEX: return "Solarize";
    }
    assert(false and "Unknown theme");
}

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

        static auto theme_index = YELLOW_THEME_INDEX;
        auto theme = themeSettings(theme_index);
        
        GUI90_WidgetBackground(theme);
        auto x = 16;
        auto y = 16;
        if (GUI90_WidgetLabel(x, y, "Label", theme)) {
        }
        y += 16;
        if (GUI90_WidgetButton(x, y, "Button", theme)) {
        }
        y += 16;
        static int setting = 0;
        setting = GUI90_WidgetIntSetting(x, y, "Setting", setting, 0, 10, theme);
        y += 32;

        auto width = 8 * 13;
        auto height = 8 * (THEME_COUNT + 2);
        
        GUI90_WidgetSelectionBoxInit(x, y, width, height, theme);
        for (auto i = 0; i < THEME_COUNT; ++i) {
            auto local_theme_index = (GuiThemeIndex)i;
            auto theme_description = themeDescription(local_theme_index);
            if (GUI90_WidgetSelectionBoxItem(theme_description, theme_index == local_theme_index)) {
                theme_index = local_theme_index;
            }
        }
        y += height;
        static auto alternative = 0;
        if (GUI90_WidgetRadioButton(x, y, "Alternative A", alternative == 0, theme)) {
            alternative = 0;
        }
        y += 16;
        if (GUI90_WidgetRadioButton(x, y, "Alternative B", alternative == 1, theme)) {
            alternative = 1;
        }
        
        sdl.draw(GUI90_GetPixelData());
    }
    return 0;
}
