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
        auto x = 2 * GUI90_BLOCK;
        auto y = 2 * GUI90_BLOCK;
        auto label = GUI90_WidgetLabel(x, y, "Label", theme);
        y += label.height;
        y += GUI90_BLOCK;
        auto open_button = GUI90_WidgetButton(x, y, "Open ", theme);
        if (open_button.is_clicked) {
        }
        y += open_button.height;
        auto close_button = GUI90_WidgetButton(x, y, "Close", theme);
        if (close_button.is_clicked) {
        }
        y += close_button.height;
        static int setting = 0;
        auto setting_widget = GUI90_WidgetIntSetting(x, y, "Setting", setting, 0, 10, theme);
        if (setting_widget.is_decreased) {
            if (0 < setting) {
                setting--;
            }
        }
        if (setting_widget.is_increased) {
            if (setting < 10) {
                setting++;
            }
        }
        y += setting_widget.height;
        y += GUI90_BLOCK;

        auto width = 13 * GUI90_BLOCK;
        auto height = (THEME_COUNT + 2) * GUI90_BLOCK;
        auto selection_box = GUI90_WidgetSelectionBoxInit(x, y, width, height, theme);
        for (auto i = 0; i < THEME_COUNT; ++i) {
            auto local_theme_index = (GuiThemeIndex)i;
            auto theme_description = themeDescription(local_theme_index);
            auto item = GUI90_WidgetSelectionBoxItem(theme_description, theme_index == local_theme_index);
            if (item.is_clicked) {
                theme_index = local_theme_index;
            }
        }
        y += selection_box.height;
        y += GUI90_BLOCK;
        static auto alternative = 0;
        auto radio_button_a = GUI90_WidgetRadioButton(x, y, "Alternative A", alternative == 0, theme);
        if (radio_button_a.is_clicked) {
            alternative = 0;
        }
        y += radio_button_a.height;
        auto radio_button_b = GUI90_WidgetRadioButton(x, y, "Alternative B", alternative == 1, theme);
        if (radio_button_b.is_clicked) {
            alternative = 1;
        }
        
        sdl.draw(GUI90_GetPixelData());
    }
    return 0;
}
