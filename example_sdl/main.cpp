#define SDL_MAIN_HANDLED

#include <gui90/gui.h>

#include "sdl_wrappers.hpp"

enum GuiThemeIndex {
    GRAY_THEME_INDEX,
    WARM_GRAY_THEME_INDEX,
    SOLARIZE_LIGHT_THEME_INDEX,
    YELLOW_THEME_INDEX,
    LEATHER_THEME_INDEX,
    THEME_COUNT,
};

GUI90_Theme themeSettings[] = {
    [GRAY_THEME_INDEX] = GUI90_THEME_GRAY,
    [WARM_GRAY_THEME_INDEX] = GUI90_THEME_WARM_GRAY,
    [SOLARIZE_LIGHT_THEME_INDEX] = GUI90_THEME_SOLARIZE_LIGHT,
    [YELLOW_THEME_INDEX] = GUI90_THEME_YELLOW,
    [LEATHER_THEME_INDEX] = GUI90_THEME_LEATHER,
};

const char* themeDescription[] = {
    [GRAY_THEME_INDEX] = "Gray",
    [WARM_GRAY_THEME_INDEX] = "Warm Gray",
    [SOLARIZE_LIGHT_THEME_INDEX] = "Solarize",
    [YELLOW_THEME_INDEX] = "Yellow",
    [LEATHER_THEME_INDEX] = "Leather",
};

int main() {
    auto WINDOW_TITLE = "GUI90";
    auto WIDTH = 320;
    auto HEIGHT = 200;
    
    auto sdl = Sdl(WINDOW_TITLE, WIDTH, HEIGHT);
    GUI90_Init(WIDTH, HEIGHT);
    sdl.setMouseModeAbsolute();
    
    for (;;) {
        auto event = SDL_Event();
        char input_character = '\0';
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
            if (event.type == SDL_TEXTINPUT) {
                input_character = event.text.text[0];
            }
        }
        auto input = sdl.getInput();
        if (input.escape_button == BUTTON_CLICKED) {
            break;
        }
        GUI90_SetMouseState(input.mouse_x, input.mouse_y, input.isLeftMouseButtonDown());
        GUI90_SetKeyboardState(
            input.keyboard[SDL_SCANCODE_LEFT],
            input.keyboard[SDL_SCANCODE_RIGHT],
            input.keyboard[SDL_SCANCODE_BACKSPACE],
            input.keyboard[SDL_SCANCODE_DELETE],
            input_character
        );
        
        static auto theme_index = YELLOW_THEME_INDEX;
        static auto button_type = BUTTON_TYPE_BEVEL;
        auto theme = themeSettings[theme_index];
        theme.button_type = button_type;
        GUI90_SetTheme(theme);
        
        GUI90_WidgetBackground();
        auto x = 2 * GUI90_BLOCK;
        auto y = 2 * GUI90_BLOCK;
        static auto label = (GUI90_Label){.x=x, .y=y, .text="Label"};
        GUI90_Update(label);
        y += label.height;
        y += GUI90_BLOCK;
        auto open_button = (GUI90_Button){.x=x, .y=y, .text="Open "};
        GUI90_Update(open_button);
        if (open_button.is_clicked) {
        }
        y += open_button.height;
        auto close_button = (GUI90_Button){.x=x, .y=y, .text="Close"};
        GUI90_Update(close_button);
        if (close_button.is_clicked) {
        }
        y += close_button.height;

        static int setting = 0;
        char setting_text[64];
        sprintf(setting_text, "Setting: %d ", setting);
        auto stepper = (GUI90_Stepper){.x=x, .y=y, .text=setting_text};
        GUI90_Update(stepper);
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
        auto selection_box = (GUI90_SelectionBoxInit){.x=x, .y=y, .width=width, .height=height};
        GUI90_Update(selection_box);
        for (auto i = 0; i < THEME_COUNT; ++i) {
            auto local_theme_index = (GuiThemeIndex)i;
            auto theme_description = themeDescription[local_theme_index];
            auto item = (GUI90_SelectionBoxItem){
                .text=theme_description, .is_selected=theme_index == local_theme_index
            };
            GUI90_Update(item);
            if (item.is_clicked) {
                theme_index = local_theme_index;
            }
        }
        y += selection_box.height;
        y += GUI90_BLOCK;
        auto radio_button_a = (GUI90_RadioButton){
            .x=x, .y=y, .text="Bevel Buttons", .is_selected=button_type == BUTTON_TYPE_BEVEL
        };
        GUI90_Update(radio_button_a);
        if (radio_button_a.is_clicked) {
            button_type = BUTTON_TYPE_BEVEL;
        }
        y += radio_button_a.height;
        auto radio_button_b = (GUI90_RadioButton) {
            .x=x, .y=y, .text="Cloud Buttons", .is_selected=button_type == BUTTON_TYPE_CLOUD
        };
        GUI90_Update(radio_button_b);
        if (radio_button_b.is_clicked) {
            button_type = BUTTON_TYPE_CLOUD;
        }

        // Second Column
        x = WIDTH / 2;
        y = 2 * GUI90_BLOCK;
        
        auto WHITE = GUI90_RGB(255, 255, 255);
        auto BLACK = GUI90_RGB(0, 0, 0);
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
        auto header = (GUI90_HeaderLabel){.x=x, .y=y, .text="Header LABEL", .theme=header_theme};
        GUI90_Update(header);

        y += header.height + GUI90_BLOCK;
        static auto input0 = (GUI90_TextInput){.x=x, .y=y, .text="Input 0"};
        GUI90_Update(input0);
        y += input0.height;
        static auto input1 = (GUI90_TextInput){.x=x, .y=y, .text="Input 1"};
        GUI90_Update(input1);
        
        sdl.draw(GUI90_GetPixelData());
    }
    return 0;
}
