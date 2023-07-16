#include "gui.hpp"

#include <string>
#include <vector>

#include "text.hpp"

struct Gui90 {
    std::vector<Color> colors;
    int width = 0;
    int height = 0;
    int mouse_x = 0;
    int mouse_y = 0;
    ButtonState left_mouse_button = ButtonState::UP;
};

// -----------------------------------------------------------------------------
// PRIVATE STUFF

static const int TEXT_SIZE = 8;
static const int BUTTON_TEXT_PADDING = 4;

struct Rectangle {
    int x;
    int y;
    int width;
    int height;
};

// -----------------------------------------------------------------------------
// PRIVATE MOUSE FUNCTIONS

static bool isLeftMouseButtonDownInside(const Gui90* gui, Rectangle r) {
    return gui->left_mouse_button == ButtonState::DOWN &&
        r.x <= gui->mouse_x && gui->mouse_x < r.x + r.width &&
        r.y <= gui->mouse_y && gui->mouse_y < r.y + r.height;
}

static bool isLeftMouseButtonReleasedInside(const Gui90* gui, Rectangle r) {
    return gui->left_mouse_button == ButtonState::RELEASED &&
        r.x <= gui->mouse_x && gui->mouse_x < r.x + r.width &&
        r.y <= gui->mouse_y && gui->mouse_y < r.y + r.height;
}

// -----------------------------------------------------------------------------
// PRIVATE DRAW FUNCTIONS

static void drawPoint(Gui90* gui, int x, int y, Color color) {
    gui->colors.at(y * gui->width + x) = color;
}

static void drawRectangle(Gui90* gui, Rectangle rectangle, Color color) {
    for (auto dy = 0; dy < rectangle.height; ++dy) {
        for (auto dx = 0; dx < rectangle.width; ++dx) {
            drawPoint(gui, rectangle.x + dx, rectangle.y + dy, color);
        }
    }
}

static void drawLineHorizontal(Gui90* gui, int x, int y, int width, Color color) {
    auto r = Rectangle{};
    r.x = x;
    r.y = y;
    r.width = width;
    r.height = 1;
    drawRectangle(gui, r, color);
}

static void drawLineVertical(Gui90* gui, int x, int y, int height, Color color) {
    auto r = Rectangle{};
    r.x = x;
    r.y = y;
    r.width = 1;
    r.height = height;
    drawRectangle(gui, r, color);
}

static void drawCharacter(Gui90* gui, char character, size_t x_start, size_t y_start, Color color) {
    const auto W = gui->width;
    const auto& digit_bitmap = text_bitmaps::get(character);
    for (size_t y = 0; y < 8; ++y) {
        for (size_t x = 0; x < 8; ++x) {
            if (digit_bitmap[y * 8 + x]) {
                gui->colors.at((y_start + y) * W + x_start + x) = color;
            }
        }
    }
}

static void drawString(Gui90* gui, const std::string& s, size_t x, size_t y, Color color) {
    for (size_t i = 0; i < s.size(); ++i) {
        drawCharacter(gui, s[i], x + 8 * i, y, color);
    }
}

static Color packColorRgb(uint32_t r, uint32_t g, uint32_t b) {
    return (255 << 24) | (r << 16) | (g << 8) | (b << 0);
}

// -----------------------------------------------------------------------------
// PUBLIC FUNCTIONS

Gui90* GUI90_Init(int width, int height) {
    auto gui = new Gui90{};
    gui->width = width;
    gui->height = height;
    gui->colors.resize(width * height);
    return gui;
}

void GUI90_Destroy(Gui90* gui) {
    delete gui;
}

void GUI90_SetMouseState(Gui90* gui, int x, int y, ButtonState left_mouse_button) {
    gui->mouse_x = x;
    gui->mouse_y = y;
    gui->left_mouse_button = left_mouse_button;
}

const Color* GUI90_GetPixelData(const Gui90* gui) {
    return gui->colors.data();
}

void GUI90_WidgetBackground(Gui90* gui, ColorShades shades) {
    for (auto& pixel : gui->colors) {
        pixel = shades.background;
    }    
}

bool GUI90_WidgetLabel(Gui90* gui, int x, int y, const char* text, ColorShades shades) {
    const auto s = std::string{text};
    auto rectangle = Rectangle{};
    rectangle.x = x;
    rectangle.y = y;
    rectangle.width = 8 * static_cast<int>(s.size());
    rectangle.height = 8;
    drawString(gui, s, x, y, shades.foreground);
    return isLeftMouseButtonReleasedInside(gui, rectangle);
}

bool GUI90_WidgetButton(Gui90* gui, int x, int y, const char* text, ColorShades shades) {
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
    if (isLeftMouseButtonDownInside(gui, rectangle)) {
        ++text_y;
        shades.bevel_light = shades.background;
        shades.bevel_dark = shades.background;
    }
    drawRectangle(gui, rectangle, shades.border);
    drawRectangle(gui, inner_rectangle, shades.background);
    drawLineHorizontal(gui, x + 2, y + 1, rectangle.width - 4, shades.bevel_light);
    drawLineHorizontal(gui, x + 2, y + rectangle.height - 2, rectangle.width - 4, shades.bevel_dark);
    drawLineVertical(gui, x + 1, y + 2, rectangle.height - 4, shades.bevel_light);
    drawLineVertical(gui, x + rectangle.width - 2, y + 2, rectangle.height - 4, shades.bevel_dark);
    drawString(gui, s, text_x, text_y, shades.foreground);
    return isLeftMouseButtonReleasedInside(gui, rectangle);
}

void GUI90_WidgetIntSetting(Gui90* gui, int x, int y, const char* text, int* value, ColorShades label_shades, ColorShades button_shades) {
    const auto label = std::string{text} + " " + std::to_string(*value) + " ";
    auto offset = 0;
    GUI90_WidgetLabel(gui, x + offset, y + BUTTON_TEXT_PADDING, label.c_str(), label_shades);
    offset += TEXT_SIZE * label.size();
    if (GUI90_WidgetButton(gui, x + offset, y, "-", button_shades)) {
        *value -= 1; 
    }
    offset += TEXT_SIZE + 2 * BUTTON_TEXT_PADDING;
    if (GUI90_WidgetButton(gui, x + offset, y, "+", button_shades)) {
        *value += 1;
    }
}

// -----------------------------------------------------------------------------
// PUBLIC CONSTANTS

const ColorShades GUI90_YELLOW_SHADES = ColorShades{
    packColorRgb(0, 0, 0),
    packColorRgb(255, 221, 63),
    packColorRgb(255, 245, 197),
    packColorRgb(207, 117, 43),
    packColorRgb(0, 0, 0),
};

const ColorShades GUI90_GRAY_SHADES = ColorShades{
    packColorRgb(255, 255, 255),
    packColorRgb(128, 128, 128),
    packColorRgb(192, 192, 192),
    packColorRgb(80, 80, 80),
    packColorRgb(0, 0, 0),
};

const ColorShades GUI90_LEATHER_SHADES = ColorShades{
    packColorRgb(0, 0, 0),
    packColorRgb(70, 50, 40),
    packColorRgb(95, 80, 73),
    packColorRgb(54, 33, 22),
    packColorRgb(0, 0, 0),
};
