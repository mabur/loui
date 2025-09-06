#define SDL_MAIN_HANDLED

#include <loui/loui.h>
#include <SDL2/SDL.h>

#include "window.hpp"
#include "input.hpp"

enum GuiThemeIndex {
    GRAY_THEME_INDEX,
    ASEPRITE_THEME_INDEX,
    FLAT_ASEPRITE_THEME_INDEX,
    WARM_GRAY_THEME_INDEX,
    DARK_GRAY_THEME_INDEX,
    SOLARIZED_LIGHT_THEME_INDEX,
    YELLOW_THEME_INDEX,
    LEATHER_THEME_INDEX,
    FLAT_WHITE_THEME_INDEX,
    FLAT_SOLARIZED_THEME_INDEX,
    THEME_COUNT,
};

LouiTheme themeSettings[] = {
    [GRAY_THEME_INDEX] = LOUI_THEME_GRAY,
    [ASEPRITE_THEME_INDEX] = LOUI_THEME_ASEPRITE,
    [FLAT_ASEPRITE_THEME_INDEX] = LOUI_THEME_FLAT_ASEPRITE,
    [WARM_GRAY_THEME_INDEX] = LOUI_THEME_WARM_GRAY,
    [DARK_GRAY_THEME_INDEX] = LOUI_THEME_DARK_GRAY,
    [SOLARIZED_LIGHT_THEME_INDEX] = LOUI_THEME_SOLARIZED_LIGHT,
    [YELLOW_THEME_INDEX] = LOUI_THEME_YELLOW,
    [LEATHER_THEME_INDEX] = LOUI_THEME_LEATHER,
    [FLAT_WHITE_THEME_INDEX] = LOUI_THEME_FLAT_WHITE,
    [FLAT_SOLARIZED_THEME_INDEX] = LOUI_THEME_FLAT_SOLARIZED,
};

const char* themeDescription[] = {
    [GRAY_THEME_INDEX] = "Gray",
    [ASEPRITE_THEME_INDEX] = "Aseprite",
    [FLAT_ASEPRITE_THEME_INDEX] = "Flat Aseprite",
    [WARM_GRAY_THEME_INDEX] = "Warm Gray",
    [DARK_GRAY_THEME_INDEX] = "Dark Gray",
    [SOLARIZED_LIGHT_THEME_INDEX] = "Solarized",
    [YELLOW_THEME_INDEX] = "Yellow",
    [LEATHER_THEME_INDEX] = "Leather",
    [FLAT_WHITE_THEME_INDEX] = "Flat White",
    [FLAT_SOLARIZED_THEME_INDEX] = "Flat Solarized",
};

void updateInput(void);
const LouiColor* updateGui(int WIDTH, int HEIGHT);

int main() {
    auto WINDOW_TITLE = "Loui";
    auto WIDTH = 320;
    auto HEIGHT = 200;

    auto window = makeDesktopWindow(WIDTH, HEIGHT, 5, WINDOW_TITLE);
    //auto window = makeFullScreenWindow(WIDTH, HEIGHT, WINDOW_TITLE);
    loui_init(WIDTH, HEIGHT);

    for (;;) {
        registerFrameInput(window.renderer);
        if (isKeyClicked(SDL_SCANCODE_ESCAPE) || hasReceivedQuitEvent()) {
            break;
        }
        updateInput();
        auto screen = updateGui(WIDTH, HEIGHT);
        drawPixels(window, screen);
        presentWindow(window);
    }
    return 0;
}

void updateInput() {
    LouiInput loui_input = {};
    loui_input.mouse_x = getAbsoluteMousePosition().x;
    loui_input.mouse_y = getAbsoluteMousePosition().y;
    loui_input.mouse_wheel_y = getMouseWheelY();
    loui_input.is_left_mouse_button_down = isLeftMouseButtonDown();
    loui_input.input_character = getInputCharacter();
    loui_input.is_keyboard_key_down[LOUI_KEY_ARROW_LEFT] = isKeyDown(SDL_SCANCODE_LEFT);
    loui_input.is_keyboard_key_down[LOUI_KEY_ARROW_RIGHT] = isKeyDown(SDL_SCANCODE_RIGHT);
    loui_input.is_keyboard_key_down[LOUI_KEY_ARROW_UP] = isKeyDown(SDL_SCANCODE_UP);
    loui_input.is_keyboard_key_down[LOUI_KEY_ARROW_DOWN] = isKeyDown(SDL_SCANCODE_DOWN);
    loui_input.is_keyboard_key_down[LOUI_KEY_BACKSPACE] = isKeyDown(SDL_SCANCODE_BACKSPACE);
    loui_input.is_keyboard_key_down[LOUI_KEY_DELETE] = isKeyDown(SDL_SCANCODE_DELETE);
    loui_input.is_keyboard_key_down[LOUI_KEY_ENTER] = isKeyDown(SDL_SCANCODE_RETURN);
    loui_input.is_keyboard_key_down[LOUI_KEY_HOME] = isKeyDown(SDL_SCANCODE_HOME);
    loui_input.is_keyboard_key_down[LOUI_KEY_END] = isKeyDown(SDL_SCANCODE_END);
    loui_input.is_keyboard_key_down[LOUI_KEY_PAGE_UP] = isKeyDown(SDL_SCANCODE_PAGEUP);
    loui_input.is_keyboard_key_down[LOUI_KEY_PAGE_DOWN] = isKeyDown(SDL_SCANCODE_PAGEDOWN);
    loui_input.is_modifier_key_down[LOUI_MODIFIER_KEY_SHIFT] = isKeyDown(SDL_SCANCODE_LSHIFT) or isKeyDown(SDL_SCANCODE_RSHIFT);
    loui_set_input(loui_input);
}

const LouiColor* updateGui(int WIDTH, int HEIGHT) {
    static auto show_window = false;
    if (show_window) {
        loui_disable_input();
    }

    static auto theme_index = ASEPRITE_THEME_INDEX;
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

    auto width = 16 * LOUI_BLOCK;
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
    auto heading_theme = LouiHeadingTheme{
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
    auto heading = (LouiHeading){.x=x, .y=y, .text="Heading", .theme=heading_theme};
    loui_update(heading);

    y += heading.height + LOUI_BLOCK;
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
    y += check_box1.height;

    static auto knob = (LouiKnob){.x=x, .y=y};
    loui_update(knob);
    x += knob.width + LOUI_BLOCK;

    static auto slider = (LouiSlider){.x=x, .y=y, .width=64, .value=0.5f};
    loui_update(slider);

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
