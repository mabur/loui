#include "gui.hpp"

#include <string.h>
#include <string>
#include <vector>

#include "text.h"

typedef struct Gui90 {
    std::vector<GUI90_Color> colors;
    int width = 0;
    int height = 0;
    int mouse_x = 0;
    int mouse_y = 0;
    int current_x = 0;
    int current_y = 0;
    GUI90_Theme theme = GUI90_THEME_GRAY; 
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

static void drawSpecialString(const std::string& s, int x, int y, GUI90_HeaderLabelTheme theme) {
    int N = s.size();
    for (int i = 0; i < N; ++i) {
        if (theme.draw_up_left) {
            drawCharacter(s[i], x - 1 + 8 * i, y - 1, theme.color_up_left);
        }
        if (theme.draw_up_right) {
            drawCharacter(s[i], x + 1 + 8 * i, y - 1, theme.color_up_right);
        }
        if (theme.draw_down_left) {
            drawCharacter(s[i], x - 1 + 8 * i, y + 1, theme.color_down_left);
        }
        if (theme.draw_down_right) {
            drawCharacter(s[i], x + 1 + 8 * i, y + 1, theme.color_down_right);
        }
        
        if (theme.draw_up) {
            drawCharacter(s[i], x + 0 + 8 * i, y - 1, theme.color_up);
        }
        if (theme.draw_left) {
            drawCharacter(s[i], x - 1 + 8 * i, y + 0, theme.color_left);
        }
        if (theme.draw_right) {
            drawCharacter(s[i], x + 1 + 8 * i, y + 0, theme.color_right);
        }
        if (theme.draw_down) {
            drawCharacter(s[i], x + 0 + 8 * i, y + 1, theme.color_down);
        }
        
        if (theme.draw_center) {
            drawCharacter(s[i], x + 0 + 8 * i, y + 0, theme.color_center);
        }
    }
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

void GUI90_SetTheme(GUI90_Theme theme) {
    s_gui.theme = theme;
}

const GUI90_Color* GUI90_GetPixelData() {
    return s_gui.colors.data();
}

GUI90_Widget GUI90_WidgetBackground() {
    for (auto& pixel : s_gui.colors) {
        pixel = s_gui.theme.background;
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

GUI90_Widget GUI90_WidgetLabel(int x, int y, const char* text) {
    auto s = std::string{text};
    drawString(s, x, y, s_gui.theme.text);
    auto rectangle = textRectangle(x, y, text);
    return GUI90_Widget{
        .width = rectangle.width,
        .height = rectangle.height,
        .is_clicked = isLeftMouseButtonReleasedInside(rectangle), 
    };
}

GUI90_Widget GUI90_WidgetHeaderLabel(int x, int y, const char* text, GUI90_HeaderLabelTheme theme) {
    auto s = std::string{text};
    drawSpecialString(s, x, y, theme);
    auto rectangle = textRectangle(x, y, text);
    return GUI90_Widget{
        .width = rectangle.width,
        .height = rectangle.height,
        .is_clicked = isLeftMouseButtonReleasedInside(rectangle),
    };
}

GUI90_Widget GUI90_WidgetButtonBevel(int x, int y, const char* text) {
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
    auto theme = s_gui.theme;
    if (isLeftMouseButtonDownInside(rectangle)) {
        ++text_y;
        theme.button_bevel_light = s_gui.theme.button_background;
        theme.button_bevel_dark = s_gui.theme.button_background;
    }
    
    drawRectangle(rectangle, s_gui.theme.button_border);
    
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

GUI90_Widget GUI90_WidgetButtonCloud(int x, int y, const char* text) {
    x += 1;
    y += 1;
    const auto s = std::string{text};
    auto rectangle = Rectangle{};
    rectangle.x = x;
    rectangle.y = y;
    rectangle.width = 8 * static_cast<int>(s.size()) + 2 * BUTTON_TEXT_PADDING;
    rectangle.height = 8 + 2 * BUTTON_TEXT_PADDING - 1;

    if (isLeftMouseButtonDownInside(rectangle)) {
        rectangle.y += 1;
    }
    
    auto inner_rectangle = rectangle;
    inner_rectangle.x += 1;
    inner_rectangle.y += 1;
    inner_rectangle.width -= 2;
    inner_rectangle.height -= 2;
    auto text_x = x + BUTTON_TEXT_PADDING;
    auto text_y = y + BUTTON_TEXT_PADDING - 1;
    auto theme = s_gui.theme;
    if (isLeftMouseButtonDownInside(rectangle)) {
        ++text_y;
    }

    // Shadow:
    drawPoint(x, y + rectangle.height - 1, theme.button_border);
    drawPoint(x + rectangle.width - 1, y + rectangle.height - 1, theme.button_border);
    drawLineHorizontal(x + 1, y + rectangle.height, rectangle.width - 2, theme.button_border);
    
    //drawRectangle(rectangle, s_gui.theme.button_border);
    drawRectangle(inner_rectangle, theme.button_background);
    
    // Rounded corners:
    drawLineHorizontal(rectangle.x + 1, rectangle.y, rectangle.width - 2, theme.button_border);
    drawLineHorizontal(rectangle.x + 1, rectangle.y + rectangle.height - 1, rectangle.width - 2, theme.button_border);
    drawLineVertical(rectangle.x, rectangle.y + 1, rectangle.height - 2, theme.button_border);
    drawLineVertical(rectangle.x + rectangle.width - 1, rectangle.y + 1, rectangle.height - 2, theme.button_border);
    drawPoint(rectangle.x + 1, rectangle.y + 1, theme.button_border);
    drawPoint(rectangle.x + 1, rectangle.y + rectangle.height - 2, theme.button_border);
    drawPoint(rectangle.x + + rectangle.width - 2, rectangle.y + 1, theme.button_border);
    drawPoint(rectangle.x + + rectangle.width - 2, rectangle.y + rectangle.height - 2, theme.button_border);
    
    drawPoint(rectangle.x + 2, rectangle.y + 1, theme.button_bevel_dark);
    drawPoint(rectangle.x + 1, rectangle.y + 2, theme.button_bevel_dark);
    drawPoint(rectangle.x + 1, rectangle.y + rectangle.height - 3, theme.button_bevel_dark);
    drawPoint(rectangle.x + 2, rectangle.y + rectangle.height - 2, theme.button_bevel_dark);
    drawPoint(rectangle.x + rectangle.width - 3, rectangle.y + 1, theme.button_bevel_dark);
    drawPoint(rectangle.x + rectangle.width - 2, rectangle.y + 2, theme.button_bevel_dark);
    drawPoint(rectangle.x + rectangle.width - 3, rectangle.y + rectangle.height - 2, theme.button_bevel_dark);
    drawPoint(rectangle.x + rectangle.width - 2, rectangle.y + rectangle.height - 3, theme.button_bevel_dark);
    
    drawString(s, text_x, text_y, theme.text);
    return GUI90_Widget{
        .width = rectangle.width + 2,
        .height = 2 * GUI90_BLOCK,
        .is_clicked = isLeftMouseButtonReleasedInside(rectangle),
    };
}

GUI90_Widget GUI90_WidgetButton(int x, int y, const char* text) {
    switch (s_gui.theme.button_type) {
        case BUTTON_TYPE_BEVEL: return GUI90_WidgetButtonBevel(x, y, text);
        case BUTTON_TYPE_CLOUD: return GUI90_WidgetButtonCloud(x, y, text);
        default: return GUI90_WidgetButtonBevel(x, y, text);
    }
}

GUI90_Widget GUI90_WidgetRadioButton(int x, int y, const char* text, bool is_selected) {
    for (int yi = 0; yi < 16; ++yi) {
        for (int xi = 0; xi < 16; ++xi) {
            double dx = xi - 7.5;
            double dy = yi - 7.5;
            double r2 = dx * dx + dy * dy;
            auto color = s_gui.theme.background;
            if (r2 < 6.0 * 6.0 and dx + dy < 0.0) {
                color = s_gui.theme.recess_bevel_dark;
            }
            if (r2 < 6.0 * 6.0 and dx + dy > 0.0) {
                color = s_gui.theme.recess_bevel_light;
            }
            if (r2 < 5.0 * 5.0) {
                color = s_gui.theme.recess_background;
            }
            if (r2 < 2.0 * 2.0 and is_selected) {
                color = s_gui.theme.recess_text_selected;
            }
            drawPoint(x + xi, y + yi, color);
        }
    }
    auto left_rectangle = Rectangle{x, y, 16 + BUTTON_TEXT_PADDING, 16};
    auto label_result = GUI90_WidgetLabel(
        x + 16 + BUTTON_TEXT_PADDING, y + BUTTON_TEXT_PADDING, text
    );
    return GUI90_Widget{
        .width = label_result.width + 16 + 8,
        .height = 16,
        .is_clicked = label_result.is_clicked or isLeftMouseButtonReleasedInside(left_rectangle), 
    };
}

GUI90_Widget GUI90_WidgetStepper(int x, int y, const char* text) {
    auto offset = 0;
    auto label_widget = GUI90_WidgetLabel(x + offset, y + BUTTON_TEXT_PADDING, text);
    offset += label_widget.width;
    auto decrease_button = GUI90_WidgetButton(x + offset, y, "-");
    offset += decrease_button.width;
    auto increase_button = GUI90_WidgetButton(x + offset, y, "+");
    offset += increase_button.width;
    return GUI90_Widget{
        .width = offset,
        .height = increase_button.height,
        .is_clicked = increase_button.is_clicked or decrease_button.is_clicked,
        .is_increased = increase_button.is_clicked,
        .is_decreased = decrease_button.is_clicked,
    };
}

GUI90_Widget GUI90_WidgetSelectionBoxInit(int x, int y, int width, int height) {
    s_gui.current_x = x + TEXT_SIZE;
    s_gui.current_y = y + TEXT_SIZE;

    const auto rectangle = Rectangle{x + 1, y + 1, width - 2, height - 2};
    drawRectangle(rectangle, s_gui.theme.recess_background);
    drawLineHorizontal(x + 1, y, width - 2, s_gui.theme.recess_bevel_dark);
    drawLineHorizontal(x + 1, y + height - 1, width - 2, s_gui.theme.recess_bevel_light);
    drawLineVertical(x, y + 1, height - 2, s_gui.theme.recess_bevel_dark);
    drawLineVertical(x + width - 1, y + 1, height - 2, s_gui.theme.recess_bevel_light);
    
    return GUI90_Widget{
        .width = width,
        .height = height,
        .is_clicked = false,
    };
}

GUI90_Widget GUI90_WidgetSelectionBoxItem(const char* text, bool is_selected) {
    auto global_theme = s_gui.theme;
    auto local_theme = s_gui.theme;
    local_theme.text = is_selected ? local_theme.recess_text_selected : local_theme.recess_text;
    GUI90_SetTheme(local_theme);
    auto label = GUI90_WidgetLabel(s_gui.current_x, s_gui.current_y, text);
    s_gui.current_y += label.height;
    GUI90_SetTheme(global_theme);
    return GUI90_Widget{
        .width = label.width,
        .height = label.height,
        .is_clicked = label.is_clicked,
    };
}

// -----------------------------------------------------------------------------
// PUBLIC CONSTANTS

const int GUI90_BLOCK = 8;
