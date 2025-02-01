#include "gui.hpp"

#include <string.h>
#include <string>
#include <vector>

#include "text.hpp"

typedef struct Gui90 {
    std::vector<GUI90_Color> colors;
    int width = 0;
    int height = 0;
    int mouse_x = 0;
    int mouse_y = 0;
    int current_x = 0;
    int current_y = 0;
    GUI90_Theme current_theme = GUI90_THEME_GRAY; 
    bool is_left_mouse_button_down = false;
    bool is_left_mouse_button_released = false;
} Gui90;

// -----------------------------------------------------------------------------
// PRIVATE STUFF

static const int TEXT_SIZE = 8;
static const int BUTTON_TEXT_PADDING = 3;

static Gui90 s_gui;

typedef struct Rectangle {
    int x;
    int y;
    int width;
    int height;
} Rectangle;

// -----------------------------------------------------------------------------
// PRIVATE MOUSE FUNCTIONS

static bool isLeftMouseButtonDownInside(Rectangle r) {
    return s_gui.is_left_mouse_button_down &&
        r.x <= s_gui.mouse_x && s_gui.mouse_x < r.x + r.width &&
        r.y <= s_gui.mouse_y && s_gui.mouse_y < r.y + r.height;
}

static bool isLeftMouseButtonReleasedInside(Rectangle r) {
    return s_gui.is_left_mouse_button_released &&
        r.x <= s_gui.mouse_x && s_gui.mouse_x < r.x + r.width &&
        r.y <= s_gui.mouse_y && s_gui.mouse_y < r.y + r.height;
}

// -----------------------------------------------------------------------------
// PRIVATE DRAW FUNCTIONS

static void drawPoint(int x, int y, GUI90_Color color) {
    s_gui.colors.at(y * s_gui.width + x) = color;
}

static void drawRectangle(Rectangle rectangle, GUI90_Color color) {
    for (auto dy = 0; dy < rectangle.height; ++dy) {
        for (auto dx = 0; dx < rectangle.width; ++dx) {
            drawPoint(rectangle.x + dx, rectangle.y + dy, color);
        }
    }
}

static void drawLineHorizontal(int x, int y, int width, GUI90_Color color) {
    auto r = Rectangle{};
    r.x = x;
    r.y = y;
    r.width = width;
    r.height = 1;
    drawRectangle(r, color);
}

static void drawLineVertical(int x, int y, int height, GUI90_Color color) {
    auto r = Rectangle{};
    r.x = x;
    r.y = y;
    r.width = 1;
    r.height = height;
    drawRectangle(r, color);
}

static void drawCharacter(char character, size_t x_start, size_t y_start, GUI90_Color color) {
    const auto W = s_gui.width;
    const auto character_bitmap = character_bitmap8x8(character);
    for (size_t y = 0; y < 8; ++y) {
        for (size_t x = 0; x < 8; ++x) {
            if (character_bitmap[y * 8 + x]) {
                s_gui.colors.at((y_start + y) * W + x_start + x) = color;
            }
        }
    }
}

static void drawString(const std::string& s, size_t x, size_t y, GUI90_Color color) {
    for (size_t i = 0; i < s.size(); ++i) {
        drawCharacter(s[i], x + 8 * i, y, color);
    }
}

static GUI90_Color packColorRgb(uint32_t r, uint32_t g, uint32_t b) {
    return (255 << 24) | (r << 16) | (g << 8) | (b << 0);
}

// -----------------------------------------------------------------------------
// PUBLIC FUNCTIONS

void GUI90_Init(int width, int height) {
    s_gui.width = width;
    s_gui.height = height;
    s_gui.colors.resize(width * height);
}

void GUI90_SetMouseState(int x, int y, bool is_left_mouse_button_down) {
    s_gui.mouse_x = x;
    s_gui.mouse_y = y;
    s_gui.is_left_mouse_button_released =
        s_gui.is_left_mouse_button_down && !is_left_mouse_button_down;
    s_gui.is_left_mouse_button_down = is_left_mouse_button_down;
}

const GUI90_Color* GUI90_GetPixelData() {
    return s_gui.colors.data();
}

GUI90_Widget GUI90_WidgetBackground(GUI90_Theme theme) {
    for (auto& pixel : s_gui.colors) {
        pixel = theme.background;
    }
    return GUI90_Widget{
        .width = s_gui.width,
        .height = s_gui.height,
        .is_clicked = s_gui.is_left_mouse_button_released,
    };
}

static Rectangle textRectangle(int x, int y, const char* text) {
    return Rectangle{
        .x = x,
        .y = y,
        .width = 8 * static_cast<int>(strlen(text)),
        .height = 8,    
    };
}

GUI90_Widget GUI90_WidgetLabel(int x, int y, const char* text, GUI90_Theme theme) {
    auto s = std::string{text};
    drawString(s, x, y, theme.text);
    auto rectangle = textRectangle(x, y, text);
    return GUI90_Widget {
        .width = rectangle.width,
        .height = rectangle.height,
        .is_clicked = isLeftMouseButtonReleasedInside(rectangle), 
    };
}

GUI90_Widget GUI90_WidgetButton(int x, int y, const char* text, GUI90_Theme theme) {
    x += 1;
    y += 1;
    const auto s = std::string{text};
    auto rectangle = Rectangle{};
    rectangle.x = x;
    rectangle.y = y;
    rectangle.width = 8 * static_cast<int>(s.size()) + 2 * BUTTON_TEXT_PADDING;
    rectangle.height = 8 + 2 * BUTTON_TEXT_PADDING;
    auto inner_rectangle = rectangle;
    inner_rectangle.x += 2;
    inner_rectangle.y += 2;
    inner_rectangle.width -= 4;
    inner_rectangle.height -= 4;
    auto text_x = x + BUTTON_TEXT_PADDING;
    auto text_y = y + BUTTON_TEXT_PADDING;
    if (isLeftMouseButtonDownInside(rectangle)) {
        ++text_y;
        theme.button_bevel_light = theme.button_background;
        theme.button_bevel_dark = theme.button_background;
    }
    
    drawRectangle(rectangle, theme.button_border);
    
    // Rounded corners:
    // drawLineHorizontal(rectangle.x + 1, rectangle.y, rectangle.width - 2, theme.button_border);
    // drawLineHorizontal(rectangle.x + 1, rectangle.y + rectangle.height - 1, rectangle.width - 2, theme.button_border);
    // drawLineVertical(rectangle.x, rectangle.y + 1, rectangle.height - 2, theme.button_border);
    // drawLineVertical(rectangle.x + rectangle.width - 1, rectangle.y + 1, rectangle.height - 2, theme.button_border);
    // drawPoint(rectangle.x + 1, rectangle.y + 1, theme.button_border);
    // drawPoint(rectangle.x + 1, rectangle.y + rectangle.height - 2, theme.button_border);
    // drawPoint(rectangle.x + + rectangle.width - 2, rectangle.y + 1, theme.button_border);
    // drawPoint(rectangle.x + + rectangle.width - 2, rectangle.y + rectangle.height - 2, theme.button_border);
    
    drawRectangle(inner_rectangle, theme.button_background);
    drawLineHorizontal(x + 2, y + 1, rectangle.width - 4, theme.button_bevel_light);
    drawLineHorizontal(x + 2, y + rectangle.height - 2, rectangle.width - 4, theme.button_bevel_dark);
    drawLineVertical(x + 1, y + 2, rectangle.height - 4, theme.button_bevel_light);
    drawLineVertical(x + rectangle.width - 2, y + 2, rectangle.height - 4, theme.button_bevel_dark);
    drawString(s, text_x, text_y, theme.text);
    return GUI90_Widget{
        .width = rectangle.width + 2,
        .height = rectangle.height + 2,
        .is_clicked = isLeftMouseButtonReleasedInside(rectangle),
    };
}

GUI90_Widget GUI90_WidgetRadioButton(int x, int y, const char* text, bool is_selected, GUI90_Theme theme) {
    for (int yi = 0; yi < 16; ++yi) {
        for (int xi = 0; xi < 16; ++xi) {
            double dx = xi - 7.5;
            double dy = yi - 7.5;
            double r2 = dx * dx + dy * dy;
            auto color = theme.background;
            if (r2 < 6.0 * 6.0 and dx + dy < 0.0) {
                color = theme.recess_bevel_dark;
            }
            if (r2 < 6.0 * 6.0 and dx + dy > 0.0) {
                color = theme.recess_bevel_light;
            }
            if (r2 < 5.0 * 5.0) {
                color = theme.recess_background;
            }
            if (r2 < 2.0 * 2.0 and is_selected) {
                color = theme.recess_text_selected;
            }
            drawPoint(x + xi, y + yi, color);
        }
    }
    auto left_rectangle = Rectangle{x, y, 16 + BUTTON_TEXT_PADDING, 16};
    auto label_result = GUI90_WidgetLabel(
        x + 16 + BUTTON_TEXT_PADDING, y + BUTTON_TEXT_PADDING, text, theme
    );
    return GUI90_Widget{
        .width = label_result.width + 16 + 8,
        .height = 16,
        .is_clicked = label_result.is_clicked or isLeftMouseButtonReleasedInside(left_rectangle), 
    };
}

GUI90_Widget GUI90_WidgetIntSetting(int x, int y, const char* text, int value, int min_value, int max_value, GUI90_Theme theme) {
    const auto label = std::string{text} + " " + std::to_string(value) + " ";
    auto offset = 0;
    GUI90_WidgetLabel(x + offset, y + BUTTON_TEXT_PADDING, label.c_str(), theme);
    offset += TEXT_SIZE * label.size();
    auto decrease_button = GUI90_WidgetButton(x + offset, y, "-", theme);
    offset += TEXT_SIZE + 2 * BUTTON_TEXT_PADDING + 2;
    auto increase_button = GUI90_WidgetButton(x + offset, y, "+", theme);
    return GUI90_Widget{
        .width = x + offset + increase_button.width,
        .height = increase_button.height,
        .is_clicked = increase_button.is_clicked or decrease_button.is_clicked,
        .is_increased = increase_button.is_clicked,
        .is_decreased = decrease_button.is_clicked,
    };
}

GUI90_Widget GUI90_WidgetSelectionBoxInit(int x, int y, int width, int height, GUI90_Theme theme) {
    s_gui.current_x = x + TEXT_SIZE;
    s_gui.current_y = y + TEXT_SIZE;
    s_gui.current_theme = theme;

    const auto rectangle = Rectangle{x + 1, y + 1, width - 2, height - 2};
    drawRectangle(rectangle, theme.recess_background);
    drawLineHorizontal(x + 1, y, width - 2, theme.recess_bevel_dark);
    drawLineHorizontal(x + 1, y + height - 1, width - 2, theme.recess_bevel_light);
    drawLineVertical(x, y + 1, height - 2, theme.recess_bevel_dark);
    drawLineVertical(x + width - 1, y + 1, height - 2, theme.recess_bevel_light);
    
    return GUI90_Widget{
        .width = width,
        .height = height,
        .is_clicked = false,
    };
}

GUI90_Widget GUI90_WidgetSelectionBoxItem(const char* text, bool is_selected) {
    auto colors = s_gui.current_theme;
    colors.text = is_selected ? colors.recess_text_selected : colors.recess_text;
    auto label = GUI90_WidgetLabel(s_gui.current_x, s_gui.current_y, text, colors);
    s_gui.current_y += label.height;
    return GUI90_Widget{
        .width = label.width,
        .height = label.height,
        .is_clicked = label.is_clicked,
    };
}

// -----------------------------------------------------------------------------
// PUBLIC CONSTANTS

const GUI90_Theme GUI90_THEME_YELLOW = GUI90_Theme{
    .background = packColorRgb(255, 221, 63),
    .text = packColorRgb(0, 0, 0),
    .button_background = packColorRgb(255, 236, 145),
    .button_text = packColorRgb(0, 0, 0),
    .button_border = packColorRgb(0, 0, 0),
    .button_bevel_dark = packColorRgb(207, 117, 43),
    .button_bevel_light = packColorRgb(255, 255, 255),
    .recess_background = packColorRgb(0, 0, 0),
    .recess_text = packColorRgb(128, 128, 128),
    .recess_text_selected = packColorRgb(255, 255, 255),
    .recess_bevel_dark = packColorRgb(207, 117, 43),
    .recess_bevel_light = packColorRgb(255, 255, 255),
};

const GUI90_Theme GUI90_THEME_GRAY = GUI90_Theme{
    .background = packColorRgb(192, 192, 192),
    .text = packColorRgb(0, 0, 0),
    .button_background = packColorRgb(192, 192, 192),
    .button_text = packColorRgb(255, 255, 255),
    .button_border = packColorRgb(0, 0, 0),
    .button_bevel_dark = packColorRgb(128, 128, 128),
    .button_bevel_light = packColorRgb(255, 255, 255),
    .recess_background = packColorRgb(0, 0, 0),
    .recess_text = packColorRgb(128, 128, 128),
    .recess_text_selected = packColorRgb(255, 255, 255),
    .recess_bevel_dark = packColorRgb(80, 80, 80),
    .recess_bevel_light = packColorRgb(255, 255, 255),
};

const GUI90_Theme GUI90_THEME_WARM_GRAY = GUI90_Theme{
    .background = packColorRgb(173, 164, 149),
    .text = packColorRgb(0, 0, 0),
    .button_background = packColorRgb(173, 164, 149),
    .button_text = packColorRgb(255, 255, 255),
    .button_border = packColorRgb(0, 0, 0),
    .button_bevel_dark = packColorRgb(105, 96, 81),
    .button_bevel_light = packColorRgb(255, 255, 255),
    .recess_background = packColorRgb(0, 0, 0),
    .recess_text = packColorRgb(128, 128, 128),
    .recess_text_selected = packColorRgb(255, 255, 255),
    .recess_bevel_dark = packColorRgb(105, 96, 81),
    .recess_bevel_light = packColorRgb(255, 255, 255),
};

const GUI90_Theme GUI90_THEME_LEATHER = GUI90_Theme{
    .background = packColorRgb(70, 50, 40),
    .text = packColorRgb(0, 0, 0),
    .button_background = packColorRgb(128, 128, 128),
    .button_text = packColorRgb(255, 255, 255),
    .button_border = packColorRgb(0, 0, 0),
    .button_bevel_dark = packColorRgb(80, 80, 80),
    .button_bevel_light = packColorRgb(192, 192, 192),
    .recess_background = packColorRgb(0, 0, 0),
    .recess_text = packColorRgb(128, 128, 128),
    .recess_text_selected = packColorRgb(255, 255, 255),
    .recess_bevel_dark = packColorRgb(54, 33, 22),
    .recess_bevel_light = packColorRgb(95, 80, 73),
};

const GUI90_Theme GUI90_THEME_SOLARIZE_LIGHT = GUI90_Theme{
    .background = packColorRgb(253, 246, 227),
    .text = packColorRgb(0, 0, 0),
    .button_background = packColorRgb(198, 180, 158),
    .button_text = packColorRgb(0, 0, 0),
    .button_border = packColorRgb(0, 0, 0),
    .button_bevel_dark = packColorRgb(134, 116, 96),
    .button_bevel_light = packColorRgb(255, 255, 255),
    .recess_background = packColorRgb(0, 0, 0),
    .recess_text = packColorRgb(128, 128, 128),
    .recess_text_selected = packColorRgb(255, 255, 255),
    .recess_bevel_dark = packColorRgb(134, 116, 96),
    .recess_bevel_light = packColorRgb(255, 255, 255),
};

const int GUI90_BLOCK = 8;
