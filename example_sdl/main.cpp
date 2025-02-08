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
        static auto button_type = BUTTON_TYPE_BEVEL;
        auto theme = themeSettings(theme_index);
        theme.button_type = button_type;
        GUI90_SetTheme(theme);
        
        GUI90_WidgetBackground();
        auto x = 2 * GUI90_BLOCK;
        auto y = 2 * GUI90_BLOCK;
        auto label = GUI90_WidgetLabel(x, y, "Label");
        y += label.height;
        y += GUI90_BLOCK;
        auto open_button = GUI90_WidgetButton(x, y, "Open ");
        if (open_button.is_clicked) {
        }
        y += open_button.height;
        auto close_button = GUI90_WidgetButton(x, y, "Close");
        if (close_button.is_clicked) {
        }
        y += close_button.height;

        static int setting = 0;
        char setting_text[64];
        sprintf(setting_text, "Setting: %d ", setting);
        auto stepper = GUI90_WidgetStepper(x, y, setting_text);
        if (stepper.is_decreased and setting > 0) {
            setting--;
        }
        if (stepper.is_increased and setting < 10) {
            setting++;
        }
        y += stepper.height;
        y += GUI90_BLOCK;

        auto width = 13 * GUI90_BLOCK;
        auto height = (THEME_COUNT + 2) * GUI90_BLOCK;
        auto selection_box = GUI90_WidgetSelectionBoxInit(x, y, width, height);
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
        auto radio_button_a = GUI90_WidgetRadioButton(x, y, "Bevel Buttons", button_type == BUTTON_TYPE_BEVEL);
        if (radio_button_a.is_clicked) {
            button_type = BUTTON_TYPE_BEVEL;
        }
        y += radio_button_a.height;
        auto radio_button_b = GUI90_WidgetRadioButton(x, y, "Cloud Buttons", button_type == BUTTON_TYPE_CLOUD);
        if (radio_button_b.is_clicked) {
            button_type = BUTTON_TYPE_CLOUD;
        }

        // Second Column
        x = WIDTH / 2;
        y = 2 * GUI90_BLOCK;
        
        auto WHITE = GUI90_Rgb(255, 255, 255);
        auto BLACK = GUI90_Rgb(0, 0, 0);
        auto MID = GUI90_InterpolateColors(BLACK, theme.background, 128);
        auto header_theme = GUI90_HeaderLabelTheme{
            .color_up_left = MID,
            .color_up = BLACK,
            .color_up_right = MID,
            .color_left = BLACK,
            .color_center = WHITE,
            .color_right = BLACK,
            .color_down_left = MID,
            .color_down = BLACK,
            .color_down_right = MID,
            .draw_up_left = true,
            .draw_up = true,
            .draw_up_right = true,
            .draw_left = true,
            .draw_center = true,
            .draw_right = true,
            .draw_down_left = true,
            .draw_down = true,
            .draw_down_right = true,
        };
        GUI90_WidgetHeaderLabel(x, y, "Header LABEL", header_theme);
        
        sdl.draw(GUI90_GetPixelData());
    }
    return 0;
}
