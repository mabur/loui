#define SDL_MAIN_HANDLED

#include <gui90/gui.h>

#include "sdl_wrappers.hpp"

enum GuiThemeIndex {
    GRAY_THEME_INDEX,
    WARM_GRAY_THEME_INDEX,
    SOLARIZED_LIGHT_THEME_INDEX,
    YELLOW_THEME_INDEX,
    LEATHER_THEME_INDEX,
    THEME_COUNT,
};

LouiTheme themeSettings[] = {
    [GRAY_THEME_INDEX] = LOUI_THEME_GRAY,
    [WARM_GRAY_THEME_INDEX] = LOUI_THEME_WARM_GRAY,
    [SOLARIZED_LIGHT_THEME_INDEX] = LOUI_THEME_SOLARIZED_LIGHT,
    [YELLOW_THEME_INDEX] = LOUI_THEME_YELLOW,
    [LEATHER_THEME_INDEX] = LOUI_THEME_LEATHER,
};

const char* themeDescription[] = {
    [GRAY_THEME_INDEX] = "Gray",
    [WARM_GRAY_THEME_INDEX] = "Warm Gray",
    [SOLARIZED_LIGHT_THEME_INDEX] = "Solarized",
    [YELLOW_THEME_INDEX] = "Yellow",
    [LEATHER_THEME_INDEX] = "Leather",
};

LouiInput createLouiInput(Input input, char input_character) {
    return (LouiInput){
            .mouse_x=input.mouse_x,
            .mouse_y=input.mouse_y,
            .is_left_mouse_button_down=input.isLeftMouseButtonDown(),
            .is_keyboard_key_down={
                [LOUI_KEYBOARD_LEFT_ARROW]=(bool)input.keyboard[SDL_SCANCODE_LEFT],
                [LOUI_KEYBOARD_RIGHT_ARROW]=(bool)input.keyboard[SDL_SCANCODE_RIGHT],
                [LOUI_KEYBOARD_UP_ARROW]=(bool)input.keyboard[SDL_SCANCODE_UP],
                [LOUI_KEYBOARD_DOWN_ARROW]=(bool)input.keyboard[SDL_SCANCODE_DOWN],
                [LOUI_KEYBOARD_BACKSPACE]=(bool)input.keyboard[SDL_SCANCODE_BACKSPACE],
                [LOUI_KEYBOARD_DELETE]=(bool)input.keyboard[SDL_SCANCODE_DELETE],
                [LOUI_KEYBOARD_ENTER]=(bool)input.keyboard[SDL_SCANCODE_RETURN],
                [LOUI_KEYBOARD_HOME]=(bool)input.keyboard[SDL_SCANCODE_HOME],
                [LOUI_KEYBOARD_END]=(bool)input.keyboard[SDL_SCANCODE_END],
                [LOUI_KEYBOARD_PAGE_UP]=(bool)input.keyboard[SDL_SCANCODE_PAGEUP],
                [LOUI_KEYBOARD_PAGE_DOWN]=(bool)input.keyboard[SDL_SCANCODE_PAGEDOWN],
            },
            .input_character=input_character,
        };
}

int main() {
    auto WINDOW_TITLE = "GUI90";
    auto WIDTH = 320;
    auto HEIGHT = 200;
    
    auto sdl = Sdl(WINDOW_TITLE, WIDTH, HEIGHT);
    loui_init(WIDTH, HEIGHT);
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
        auto sdl_input = sdl.getInput();
        if (sdl_input.escape_button == BUTTON_CLICKED) {
            break;
        }
        auto loui_input = createLouiInput(sdl_input, input_character);
        loui_set_input(loui_input);
        
        static auto theme_index = YELLOW_THEME_INDEX;
        static auto button_type = BUTTON_TYPE_BEVEL;
        auto theme = themeSettings[theme_index];
        theme.button_type = button_type;
        loui_set_theme(theme);
        
        loui_widget_background();
        auto x = 2 * LOUI_BLOCK;
        auto y = 2 * LOUI_BLOCK;
        auto label = (LouiLabel){.x=x, .y=y, .text="Label"};
        loui_update(label);
        y += label.height;
        y += LOUI_BLOCK;
        auto open_button = (LouiButton){.x=x, .y=y, .text="Open "};
        loui_update(open_button);
        if (open_button.is_clicked) {
        }
        y += open_button.height;
        auto close_button = (LouiButton){.x=x, .y=y, .text="Close"};
        loui_update(close_button);
        if (close_button.is_clicked) {
        }
        y += close_button.height;

        static int setting = 0;
        char setting_text[64];
        sprintf(setting_text, "Setting: %d ", setting);
        auto stepper = (LouiStepper){.x=x, .y=y, .text=setting_text};
        loui_update(stepper);
        if (stepper.is_decreased and setting > 0) {
            setting--;
        }
        if (stepper.is_increased and setting < 10) {
            setting++;
        }
        y += stepper.height;
        y += LOUI_BLOCK;

        auto width = 13 * LOUI_BLOCK;
        auto height = (THEME_COUNT + 2) * LOUI_BLOCK;
        auto selection_box = (LouiSelectionBoxInit){.x=x, .y=y, .width=width, .height=height};
        loui_update(selection_box);
        for (auto i = 0; i < THEME_COUNT; ++i) {
            auto local_theme_index = (GuiThemeIndex)i;
            auto theme_description = themeDescription[local_theme_index];
            auto item = (LouiSelectionBoxItem){
                .text=theme_description, .is_selected=theme_index == local_theme_index
            };
            loui_update(item);
            if (item.is_clicked) {
                theme_index = local_theme_index;
            }
        }
        y += selection_box.height;

        auto radio_button_a = (LouiRadioButton){
            .x=x, .y=y, .text="Bevel Buttons", .is_selected=button_type == BUTTON_TYPE_BEVEL
        };
        loui_update(radio_button_a);
        if (radio_button_a.is_clicked) {
            button_type = BUTTON_TYPE_BEVEL;
        }
        y += radio_button_a.height;
        auto radio_button_b = (LouiRadioButton) {
            .x=x, .y=y, .text="Cloud Buttons", .is_selected=button_type == BUTTON_TYPE_CLOUD
        };
        loui_update(radio_button_b);
        if (radio_button_b.is_clicked) {
            button_type = BUTTON_TYPE_CLOUD;
        }

        // Second Column
        x = WIDTH / 2;
        y = 2 * LOUI_BLOCK;
        
        auto WHITE = LOUI_RGB(255, 255, 255);
        auto BLACK = LOUI_RGB(0, 0, 0);
        auto MID = loui_interpolate_colors(BLACK, theme.background, 128);
        auto header_theme = LouiHeaderLabelTheme{
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
        auto header = (LouiHeaderLabel){.x=x, .y=y, .text="Header LABEL", .theme=header_theme};
        loui_update(header);

        y += header.height + LOUI_BLOCK;
        static auto input0 = (LouiTextInput){.x=x, .y=y, .text="Input 0"};
        loui_update(input0);
        y += input0.height;
        static auto input1 = (LouiTextInput){.x=x, .y=y, .text="Input 1"};
        loui_update(input1);
        y += input1.height;
        y += LOUI_BLOCK;

        static auto multi_text_input = (LouiMultiTextInput){
            .x=x, .y=y, .text="Multi\nlines of\ntext", .rows=8, .columns=16
        };
        loui_update(multi_text_input);
        y += multi_text_input.height;

        static auto check_box0 = (LouiCheckBox){.x=x, .y=y, .text="Check Box 0", .is_selected=true};
        loui_update(check_box0);
        y += check_box0.height;
        static auto check_box1 = (LouiCheckBox){.x=x, .y=y, .text="Check Box 1"};
        loui_update(check_box1);

        sdl.draw(loui_get_pixel_data());
    }
    return 0;
}
