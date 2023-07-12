#include "gui.hpp"

#include <string>
#include <vector>

#include "text.hpp"

const auto TEXT_SIZE = 8;
const auto BUTTON_TEXT_PADDING = 4;

struct Rectangle {
    int x;
    int y;
    int width;
    int height;
};

struct State {
    std::vector<Color> colors;
    int width = 0;
    int height = 0;
    int mouse_x = 0;
    int mouse_y = 0;
    ButtonState left_mouse_button = ButtonState::UP;
};

auto state = State{};

bool isLeftMouseButtonDownInside(Rectangle r) {
    return state.left_mouse_button == ButtonState::DOWN &&
        r.x <= state.mouse_x && state.mouse_x < r.x + r.width &&
        r.y <= state.mouse_y && state.mouse_y < r.y + r.height;
}

bool isLeftMouseButtonReleasedInside(Rectangle r) {
    return state.left_mouse_button == ButtonState::RELEASED &&
        r.x <= state.mouse_x && state.mouse_x < r.x + r.width &&
        r.y <= state.mouse_y && state.mouse_y < r.y + r.height;
}

void init(int width, int height) {
    state.width = width;
    state.height = height;
    state.colors.resize(width * height);
}

void setMouseState(int x, int y, ButtonState left_mouse_button) {
    state.mouse_x = x;
    state.mouse_y = y;
    state.left_mouse_button = left_mouse_button;
}

const Color* getPixelData() {
    return state.colors.data();
}

void drawPoint(int x, int y, Color color) {
    state.colors.at(y * state.width + x) = color;
}

void drawRectangle(Rectangle rectangle, Color color) {
    for (auto dy = 0; dy < rectangle.height; ++dy) {
        for (auto dx = 0; dx < rectangle.width; ++dx) {
            drawPoint(rectangle.x + dx, rectangle.y + dy, color);
        }
    }
}

void drawLineHorizontal(int x, int y, int width, Color color) {
    auto r = Rectangle{};
    r.x = x;
    r.y = y;
    r.width = width;
    r.height = 1;
    drawRectangle(r, color);
}

void drawLineVertical(int x, int y, int height, Color color) {
    auto r = Rectangle{};
    r.x = x;
    r.y = y;
    r.width = 1;
    r.height = height;
    drawRectangle(r, color);
}

void drawCharacter(char character, size_t x_start, size_t y_start, Color color) {
    const auto W = state.width;
    const auto& digit_bitmap = text_bitmaps::get(character);
    for (size_t y = 0; y < 8; ++y) {
        for (size_t x = 0; x < 8; ++x) {
            if (digit_bitmap[y * 8 + x]) {
                state.colors.at((y_start + y) * W + x_start + x) = color;
            }
        }
    }
}

void drawString(const std::string& s, size_t x, size_t y, Color color) {
    for (size_t i = 0; i < s.size(); ++i) {
        drawCharacter(s[i], x + 8 * i, y, color);
    }
}

bool guiBackground(ColorShades shades) {
    for (auto& pixel : state.colors) {
        pixel = shades.background;
    }    
}

bool guiLabel(int x, int y, const char* text, ColorShades shades) {
    const auto s = std::string{text};
    auto rectangle = Rectangle{};
    rectangle.x = x;
    rectangle.y = y;
    rectangle.width = 8 * static_cast<int>(s.size());
    rectangle.height = 8;
    drawString(s, x, y, shades.foreground);
    return isLeftMouseButtonReleasedInside(rectangle);
}

bool guiButton(int x, int y, const char* text, ColorShades shades) {
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
        shades.bevel_light = shades.background;
        shades.bevel_dark = shades.background;
    }
    drawRectangle(rectangle, shades.border);
    drawRectangle(inner_rectangle, shades.background);
    drawLineHorizontal(x + 2, y + 1, rectangle.width - 4, shades.bevel_light);
    drawLineHorizontal(x + 2, y + rectangle.height - 2, rectangle.width - 4, shades.bevel_dark);
    drawLineVertical(x + 1, y + 2, rectangle.height - 4, shades.bevel_light);
    drawLineVertical(x + rectangle.width - 2, y + 2, rectangle.height - 4, shades.bevel_dark);
    drawString(s, text_x, text_y, shades.foreground);
    return isLeftMouseButtonReleasedInside(rectangle);
}

void guiIntSetting(int x, int y, const char* text, ColorShades shades, int* value) {
    const auto label = std::string{text} + " " + std::to_string(*value) + " ";
    auto offset = 0;
    guiLabel(x + offset, y + BUTTON_TEXT_PADDING, label.c_str(), shades);
    offset += TEXT_SIZE * label.size();
    if (guiButton(x + offset, y, "-", shades)) {
        *value -= 1; 
    }
    offset += TEXT_SIZE + 2 * BUTTON_TEXT_PADDING;
    if (guiButton(x + offset, y, "+", shades)) {
        *value += 1;
    }
}
