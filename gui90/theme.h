#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "color.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum LouiButtonType {
    BUTTON_TYPE_BEVEL,
    BUTTON_TYPE_CLOUD,
} LouiButtonType;

typedef struct LouiTheme {
    LouiColor background;
    LouiColor text;
    LouiColor button_background;
    LouiColor button_text;
    LouiColor button_border;
    LouiColor button_bevel_dark;
    LouiColor button_bevel_light;
    LouiColor recess_background;
    LouiColor recess_text;
    LouiColor recess_text_selected;
    LouiColor recess_bevel_dark;
    LouiColor recess_bevel_light;
    LouiButtonType button_type;
} LouiTheme;

typedef struct LouiHeaderLabelTheme {
    LouiColor color_up_left;
    LouiColor color_up;
    LouiColor color_up_right;
    LouiColor color_left;
    LouiColor color_center;
    LouiColor color_right;
    LouiColor color_down_left;
    LouiColor color_down;
    LouiColor color_down_right;
    bool draw_up_left;
    bool draw_up;
    bool draw_up_right;
    bool draw_left;
    bool draw_center;
    bool draw_right;
    bool draw_down_left;
    bool draw_down;
    bool draw_down_right;
} LouiHeaderLabelTheme;

extern const LouiTheme LOUI_THEME_YELLOW;
extern const LouiTheme LOUI_THEME_GRAY;
extern const LouiTheme LOUI_THEME_WARM_GRAY;
extern const LouiTheme LOUI_THEME_LEATHER;
extern const LouiTheme LOUI_THEME_SOLARIZED_LIGHT;
extern const LouiTheme LOUI_THEME_FLAT_WHITE;

#ifdef __cplusplus
}
#endif
