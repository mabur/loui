#include "gui.hpp"

#include <string>
#include <vector>

#include "text.hpp"

struct Gui90 {
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
};

// -----------------------------------------------------------------------------
// PRIVATE STUFF

static const int TEXT_SIZE = 8;
static const int BUTTON_TEXT_PADDING = 4;

static Gui90 s_gui;

struct Rectangle {
    int x;
    int y;
    int width;
    int height;
};

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

void GUI90_WidgetBackground(GUI90_Theme theme) {
    for (auto& pixel : s_gui.colors) {
        pixel = theme.background;
    }    
}

bool GUI90_WidgetLabel(int x, int y, const char* text, GUI90_Theme theme) {
    const auto s = std::string{text};
    auto rectangle = Rectangle{};
    rectangle.x = x;
    rectangle.y = y;
    rectangle.width = 8 * static_cast<int>(s.size());
    rectangle.height = 8;
    drawString(s, x, y, theme.text);
    return isLeftMouseButtonReleasedInside(rectangle);
}

bool GUI90_WidgetButton(int x, int y, const char* text, GUI90_Theme theme) {
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
        theme.bevel_light = theme.background;
        theme.bevel_dark = theme.background;
    }
    drawRectangle(rectangle, theme.border);
    drawRectangle(inner_rectangle, theme.background);
    drawLineHorizontal(x + 2, y + 1, rectangle.width - 4, theme.bevel_light);
    drawLineHorizontal(x + 2, y + rectangle.height - 2, rectangle.width - 4, theme.bevel_dark);
    drawLineVertical(x + 1, y + 2, rectangle.height - 4, theme.bevel_light);
    drawLineVertical(x + rectangle.width - 2, y + 2, rectangle.height - 4, theme.bevel_dark);
    drawString(s, text_x, text_y, theme.text);
    return isLeftMouseButtonReleasedInside(rectangle);
}

int GUI90_WidgetIntSetting(int x, int y, const char* text, int value, int min_value, int max_value, GUI90_Theme label_colors, GUI90_Theme button_colors) {
    const auto label = std::string{text} + " " + std::to_string(value) + " ";
    auto offset = 0;
    GUI90_WidgetLabel(x + offset, y + BUTTON_TEXT_PADDING, label.c_str(), label_colors);
    offset += TEXT_SIZE * label.size();
    if (GUI90_WidgetButton(x + offset, y, "-", button_colors)) {
        if (min_value < value) {
            value--;
        }
    }
    offset += TEXT_SIZE + 2 * BUTTON_TEXT_PADDING;
    if (GUI90_WidgetButton(x + offset, y, "+", button_colors)) {
        if (value < max_value) {
            value++;
        }
    }
    return value;
}

void GUI90_WidgetSelectionBoxInit(int x, int y, int width, int height, GUI90_Theme theme) {
    s_gui.current_x = x + TEXT_SIZE;
    s_gui.current_y = y + TEXT_SIZE;
    s_gui.current_theme = theme;

    const auto rectangle = Rectangle{x + 1, y + 1, width - 2, height - 2};
    drawRectangle(rectangle, theme.background_dark);
    drawLineHorizontal(x + 1, y, width - 2, theme.bevel_dark);
    drawLineHorizontal(x + 1, y + height - 1, width - 2, theme.bevel_light);
    drawLineVertical(x, y + 1, height - 2, theme.bevel_dark);
    drawLineVertical(x + width - 1, y + 1, height - 2, theme.bevel_light);
}

bool GUI90_WidgetSelectionBoxItem(const char* text, bool is_selected) {
    auto colors = s_gui.current_theme;
    colors.text = is_selected ?
        colors.text_selection_box_selected :
        colors.text_selection_box;
    const auto x = s_gui.current_x;
    const auto y = s_gui.current_y;
    s_gui.current_y += TEXT_SIZE;
    return GUI90_WidgetLabel(x, y, text, colors);
}

// -----------------------------------------------------------------------------
// PUBLIC CONSTANTS

const GUI90_Theme GUI90_THEME_YELLOW = GUI90_Theme{
    .text = packColorRgb(0, 0, 0),
    .text_selection_box_selected = packColorRgb(255, 255, 255),
    .text_selection_box = packColorRgb(128, 128, 128),
    .background = packColorRgb(255, 221, 63),
    .background_dark = packColorRgb(255, 221, 63),
    .bevel_light = packColorRgb(255, 245, 197),
    .bevel_dark = packColorRgb(207, 117, 43),
    .border = packColorRgb(0, 0, 0),
};

const GUI90_Theme GUI90_THEME_GRAY = GUI90_Theme{
    .text = packColorRgb(255, 255, 255),
    .text_selection_box_selected = packColorRgb(0, 0, 0),
    .text_selection_box = packColorRgb(128, 128, 128),
    .background = packColorRgb(128, 128, 128),
    .background_dark = packColorRgb(128, 128, 128),
    .bevel_light = packColorRgb(192, 192, 192),
    .bevel_dark = packColorRgb(80, 80, 80),
    .border = packColorRgb(0, 0, 0),
};

const GUI90_Theme GUI90_THEME_LEATHER = GUI90_Theme{
    .text = packColorRgb(0, 0, 0),
    .text_selection_box_selected = packColorRgb(255, 255, 255),
    .text_selection_box = packColorRgb(128, 128, 128),
    .background = packColorRgb(70, 50, 40),
    .background_dark = packColorRgb(0, 0, 0),
    .bevel_light = packColorRgb(95, 80, 73),
    .bevel_dark = packColorRgb(54, 33, 22),
    .border = packColorRgb(0, 0, 0),
};
