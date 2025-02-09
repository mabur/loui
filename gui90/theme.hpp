#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "color.hpp"

typedef enum GUI90_ButtonType {
    BUTTON_TYPE_BEVEL,
    BUTTON_TYPE_CLOUD,
} GUI90_ButtonType;

typedef struct GUI90_Theme {
    GUI90_Color background;
    GUI90_Color text;
    GUI90_Color button_background;
    GUI90_Color button_text;
    GUI90_Color button_border;
    GUI90_Color button_bevel_dark;
    GUI90_Color button_bevel_light;
    GUI90_Color recess_background;
    GUI90_Color recess_text;
    GUI90_Color recess_text_selected;
    GUI90_Color recess_bevel_dark;
    GUI90_Color recess_bevel_light;
    GUI90_ButtonType button_type;
} GUI90_Theme;

typedef struct GUI90_HeaderLabelTheme {
    GUI90_Color color_up_left;
    GUI90_Color color_up;
    GUI90_Color color_up_right;
    GUI90_Color color_left;
    GUI90_Color color_center;
    GUI90_Color color_right;
    GUI90_Color color_down_left;
    GUI90_Color color_down;
    GUI90_Color color_down_right;
    bool draw_up_left;
    bool draw_up;
    bool draw_up_right;
    bool draw_left;
    bool draw_center;
    bool draw_right;
    bool draw_down_left;
    bool draw_down;
    bool draw_down_right;
} GUI90_HeaderLabelTheme;

extern const GUI90_Theme GUI90_THEME_YELLOW;
extern const GUI90_Theme GUI90_THEME_GRAY;
extern const GUI90_Theme GUI90_THEME_WARM_GRAY;
extern const GUI90_Theme GUI90_THEME_LEATHER;
extern const GUI90_Theme GUI90_THEME_SOLARIZE_LIGHT;
