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

LouiInput createLouiInput(Input input, char input_character, int mouse_wheel_y) {
    LouiInput loui_input = {};
    loui_input.mouse_x = input.mouse_x;
    loui_input.mouse_y = input.mouse_y;
    loui_input.mouse_wheel_y = mouse_wheel_y;
    loui_input.is_left_mouse_button_down = input.isLeftMouseButtonDown();
    loui_input.input_character = input_character;

    loui_input.is_keyboard_key_down[LOUI_KEY_ARROW_LEFT] = input.keyboard[SDL_SCANCODE_LEFT];
    loui_input.is_keyboard_key_down[LOUI_KEY_ARROW_RIGHT] = input.keyboard[SDL_SCANCODE_RIGHT];
    loui_input.is_keyboard_key_down[LOUI_KEY_ARROW_UP] = input.keyboard[SDL_SCANCODE_UP];
    loui_input.is_keyboard_key_down[LOUI_KEY_ARROW_DOWN] = input.keyboard[SDL_SCANCODE_DOWN];
    loui_input.is_keyboard_key_down[LOUI_KEY_BACKSPACE] = input.keyboard[SDL_SCANCODE_BACKSPACE];
    loui_input.is_keyboard_key_down[LOUI_KEY_DELETE] = input.keyboard[SDL_SCANCODE_DELETE];
    loui_input.is_keyboard_key_down[LOUI_KEY_ENTER] = input.keyboard[SDL_SCANCODE_RETURN];
    loui_input.is_keyboard_key_down[LOUI_KEY_HOME] = input.keyboard[SDL_SCANCODE_HOME];
    loui_input.is_keyboard_key_down[LOUI_KEY_END] = input.keyboard[SDL_SCANCODE_END];
    loui_input.is_keyboard_key_down[LOUI_KEY_PAGE_UP] = input.keyboard[SDL_SCANCODE_PAGEUP];
    loui_input.is_keyboard_key_down[LOUI_KEY_PAGE_DOWN] = input.keyboard[SDL_SCANCODE_PAGEDOWN];
    return loui_input;
}

const LouiColor* updateGui(LouiInput loui_input, int WIDTH, int HEIGHT) {
    loui_set_input(loui_input);

    static auto show_window = false;
    if (show_window) {
        loui_disable_input();
    }

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
        show_window = true;
    }
    y += open_button.height;

    auto close_button = (LouiButton){.x=x, .y=y, .text="Close"};
    loui_update(close_button);
    if (close_button.is_clicked) {
        show_window = false;
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
        .x=x,
        .y=y,
        .text="Multi\nlines of\ntext\na very very long line\nb\nc\nd\ne\nf",
        .lines=7,
        .columns=14,
        .draw_caret=(MultiLineCaret){.line=0, .column=0},
    };
    loui_update(multi_text_input);
    y += multi_text_input.height;

    static auto check_box0 = (LouiCheckBox){.x=x, .y=y, .text="Check Box 0", .is_selected=true};
    loui_update(check_box0);
    y += check_box0.height;
    static auto check_box1 = (LouiCheckBox){.x=x, .y=y, .text="Check Box 1"};
    loui_update(check_box1);

    x = 8 * 9;
    y = 8;
    static auto window = (LouiWindow){.x=x, .y=y, .width=8*9, .height=8*8};
    if (show_window) {
        loui_enable_input();
        loui_update(window);
        x += LOUI_BLOCK;
        y += LOUI_BLOCK;
        auto label = (LouiLabel){.x=x, .y=y, .text="Message"};
        loui_update(label);
        y += label.height;
        y += 1 * LOUI_BLOCK;
        auto ok_button = (LouiButton){.x=x, .y=y, .text="OK"};
        loui_update(ok_button);
        if (ok_button.is_clicked) {
            show_window = false;
        }
        y += ok_button.height;
    }

    return loui_get_pixel_data();
}

int main() {
    auto WINDOW_TITLE = "Loui";
    auto WIDTH = 320;
    auto HEIGHT = 200;
    
    auto sdl = Sdl(WINDOW_TITLE, WIDTH, HEIGHT);
    loui_init(WIDTH, HEIGHT);
    sdl.setMouseModeAbsolute();
    
    for (;;) {
        auto event = SDL_Event();
        char input_character = '\0';
        auto mouse_wheel_y = 0;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
            if (event.type == SDL_TEXTINPUT) {
                input_character = event.text.text[0];
            }
            if (event.type == SDL_MOUSEWHEEL) {
                mouse_wheel_y = event.wheel.y;
            }
        }
        auto sdl_input = sdl.getInput();
        if (sdl_input.escape_button == BUTTON_CLICKED) {
            break;
        }
        auto loui_input = createLouiInput(sdl_input, input_character, mouse_wheel_y);
        auto screen = updateGui(loui_input, WIDTH, HEIGHT);
        sdl.draw(screen);
    }
    return 0;
}
