#include "theme.h"

const LouiTheme LOUI_THEME_YELLOW = (LouiTheme){
    .background = LOUI_RGB(255, 221, 63),
    .text = LOUI_RGB(0, 0, 0),
    .button_background = LOUI_RGB(255, 255, 255),
    .button_text = LOUI_RGB(0, 0, 0),
    .button_border = LOUI_RGB(0, 0, 0),
    .button_bevel_dark = LOUI_RGB(230, 210, 180),
    .button_bevel_light = LOUI_RGB(255, 255, 255),
    .recess_background = LOUI_RGB(0, 0, 0),
    .recess_text = LOUI_RGB(128, 128, 128),
    .recess_text_selected = LOUI_RGB(255, 255, 255),
    .recess_bevel_dark = LOUI_RGB(207, 117, 43),
    .recess_bevel_light = LOUI_RGB(255, 255, 255),
    .button_type = BUTTON_TYPE_BEVEL,
};

const LouiTheme LOUI_THEME_GRAY = (LouiTheme){
    .background = LOUI_RGB(192, 192, 192),
    .text = LOUI_RGB(0, 0, 0),
    .button_background = LOUI_RGB(192, 192, 192), // LOUI_RGB(255, 255, 255),
    .button_text = LOUI_RGB(0, 0, 0),
    .button_border = LOUI_RGB(0, 0, 0),
    .button_bevel_dark = LOUI_RGB(128, 128, 128), // LOUI_RGB(192, 192, 192),
    .button_bevel_light = LOUI_RGB(255, 255, 255),
    .recess_background = LOUI_RGB(0, 0, 0),
    .recess_text = LOUI_RGB(128, 128, 128),
    .recess_text_selected = LOUI_RGB(255, 255, 255),
    .recess_bevel_dark = LOUI_RGB(128, 128, 128),
    .recess_bevel_light = LOUI_RGB(255, 255, 255),
    .button_type = BUTTON_TYPE_BEVEL,
};

const LouiTheme LOUI_THEME_ASEPRITE = (LouiTheme){
    .background = LOUI_RGB(210, 202, 189),
    .text = LOUI_RGB(0, 0, 0),
    .button_background = LOUI_RGB(198, 198, 198),
    .button_text = LOUI_RGB(0, 0, 0),
    .button_border = LOUI_RGB(0, 0, 0),
    .button_bevel_dark = LOUI_RGB(128, 128, 128),
    .button_bevel_light = LOUI_RGB(255, 255, 255),
    .recess_background = LOUI_RGB(0, 0, 0),
    .recess_text = LOUI_RGB(128, 128, 128),
    .recess_text_selected = LOUI_RGB(255, 255, 255),
    .recess_bevel_dark = LOUI_RGB(148, 131, 115),
    .recess_bevel_light = LOUI_RGB(255, 255, 255),
    .button_type = BUTTON_TYPE_BEVEL,
};

const LouiTheme LOUI_THEME_FLAT_ASEPRITE = (LouiTheme){
    .background = LOUI_RGB(210, 202, 189),
    .text = LOUI_RGB(0, 0, 0),
    .button_background = LOUI_RGB(124, 145, 157), // LOUI_RGB(198, 198, 198), // LOUI_RGB(255, 255, 255),
    .button_text = LOUI_RGB(255, 255, 255),
    .button_border = LOUI_RGB(0, 0, 0),
    .button_bevel_dark = LOUI_RGB(77, 92, 101), //LOUI_RGB(128, 128, 128), // LOUI_RGB(230, 210, 180), //
    .button_bevel_light = LOUI_RGB(144, 168, 185), // LOUI_RGB(255, 255, 255),
    .recess_background = LOUI_RGB(255, 255, 255),
    .recess_text = LOUI_RGB(96, 96, 96),
    .recess_text_selected = LOUI_RGB(0, 0, 0),
    .recess_bevel_dark = LOUI_RGB(0, 0, 0),
    .recess_bevel_light = LOUI_RGB(0, 0, 0),
    .button_type = BUTTON_TYPE_BEVEL,
};

const LouiTheme LOUI_THEME_WARM_GRAY = (LouiTheme){
    .background = LOUI_RGB(180, 168, 156),
    .text = LOUI_RGB(0, 0, 0),
    .button_background = LOUI_RGB(234, 222, 210), //LOUI_RGB(255, 255, 255),
    .button_text = LOUI_RGB(0, 0, 0),
    .button_border = LOUI_RGB(0, 0, 0),
    .button_bevel_dark = LOUI_RGB(159, 147, 135), //LOUI_RGB(192, 192, 192),
    .button_bevel_light = LOUI_RGB(255, 255, 255),
    .recess_background = LOUI_RGB(0, 0, 0),
    .recess_text = LOUI_RGB(128, 128, 128),
    .recess_text_selected = LOUI_RGB(255, 255, 255),
    .recess_bevel_dark = LOUI_RGB(105, 93, 81),
    .recess_bevel_light = LOUI_RGB(255, 255, 255),
    .button_type = BUTTON_TYPE_BEVEL,
};

const LouiTheme LOUI_THEME_DARK_GRAY = (LouiTheme){
    .background = LOUI_RGB(88, 84, 80),
    .text = LOUI_RGB(0, 0, 0), // LOUI_RGB(210, 206, 202),
    .button_background = LOUI_RGB(234, 222, 210), //LOUI_RGB(88, 84, 80),
    .button_text = LOUI_RGB(0, 0, 0), // LOUI_RGB(210, 206, 202), //
    .button_border = LOUI_RGB(0, 0, 0),
    .button_bevel_dark = LOUI_RGB(159, 147, 135), //LOUI_RGB(56, 52, 58),
    .button_bevel_light = LOUI_RGB(255, 255, 255), //LOUI_RGB(120, 116, 112),
    .recess_background = LOUI_RGB(0, 0, 0),
    .recess_text = LOUI_RGB(128, 128, 128), //LOUI_RGB(210, 206, 202),
    .recess_text_selected = LOUI_RGB(255, 255, 255),
    .recess_bevel_dark = LOUI_RGB(56, 52, 58),
    .recess_bevel_light = LOUI_RGB(120, 116, 112),
    .button_type = BUTTON_TYPE_BEVEL,
};

const LouiTheme LOUI_THEME_LEATHER = (LouiTheme){
    .background = LOUI_RGB(70, 50, 40),
    .text = LOUI_RGB(0, 0, 0),
    .button_background = LOUI_RGB(128, 128, 128),
    .button_text = LOUI_RGB(255, 255, 255),
    .button_border = LOUI_RGB(0, 0, 0),
    .button_bevel_dark = LOUI_RGB(80, 80, 80),
    .button_bevel_light = LOUI_RGB(192, 192, 192),
    .recess_background = LOUI_RGB(0, 0, 0),
    .recess_text = LOUI_RGB(128, 128, 128),
    .recess_text_selected = LOUI_RGB(255, 255, 255),
    .recess_bevel_dark = LOUI_RGB(54, 33, 22),
    .recess_bevel_light = LOUI_RGB(95, 80, 73),
    .button_type = BUTTON_TYPE_BEVEL,
};

const LouiTheme LOUI_THEME_SOLARIZED_LIGHT = (LouiTheme){
    .background = LOUI_RGB(238, 232, 213),
    .text = LOUI_RGB(39, 40, 34), // LOUI_RGB(0, 0, 0),
    .button_background =  LOUI_RGB(238, 232, 213), // LOUI_RGB(255, 255, 255),
    .button_text = LOUI_RGB(39, 40, 34), // LOUI_RGB(0, 0, 0),
    .button_border = LOUI_RGB(39, 40, 34), // LOUI_RGB(0, 0, 0),
    .button_bevel_dark = LOUI_RGB(134, 116, 96), // LOUI_RGB(192, 192, 192),
    .button_bevel_light = LOUI_RGB(255, 255, 255), // LOUI_RGB(255, 255, 255),
    .recess_background = LOUI_RGB(39, 40, 34), // LOUI_RGB(0, 0, 0),
    .recess_text = LOUI_RGB(128, 128, 128),
    .recess_text_selected = LOUI_RGB(255, 255, 255),
    .recess_bevel_dark = LOUI_RGB(134, 116, 96),
    .recess_bevel_light = LOUI_RGB(255, 255, 255),
    .button_type = BUTTON_TYPE_BEVEL,
};

const LouiTheme LOUI_THEME_FLAT_WHITE = (LouiTheme){
    .background = LOUI_RGB(255, 255, 255),
    .text = LOUI_RGB(0, 0, 0),
    .button_background = LOUI_RGB(58, 113, 236),
    .button_text = LOUI_RGB(255, 255, 255),
    .button_border = LOUI_RGB(255, 255, 255),
    .button_bevel_dark = LOUI_RGB(58, 113, 236), // LOUI_RGB(28, 75, 170),
    .button_bevel_light = LOUI_RGB(58, 113, 236), // LOUI_RGB(173, 200, 255),
    .recess_background = LOUI_RGB(249, 249, 249),
    .recess_text = LOUI_RGB(128, 128, 128),
    .recess_text_selected = LOUI_RGB(0, 0, 0),
    .recess_bevel_dark = LOUI_RGB(236, 236, 236),
    .recess_bevel_light = LOUI_RGB(236, 236, 236),
    .button_type = BUTTON_TYPE_BEVEL,
};

const LouiTheme LOUI_THEME_FLAT_SOLARIZED = (LouiTheme){
    .background = LOUI_RGB(253, 246, 227),
    .text = LOUI_RGB(68, 67, 62),
    .button_background = LOUI_RGB(58, 113, 236),
    .button_text = LOUI_RGB(255, 255, 255),
    .button_border = LOUI_RGB(253, 246, 227),
    .button_bevel_dark = LOUI_RGB(58, 113, 236), // LOUI_RGB(28, 75, 170),
    .button_bevel_light = LOUI_RGB(58, 113, 236), // LOUI_RGB(173, 200, 255),
    .recess_background = LOUI_RGB(238, 232, 213),
    .recess_text = LOUI_RGB(128, 128, 128),
    .recess_text_selected = LOUI_RGB(68, 67, 62),
    .recess_bevel_dark = LOUI_RGB(205, 200, 183),
    .recess_bevel_light = LOUI_RGB(205, 200, 183),
    .button_type = BUTTON_TYPE_BEVEL,
};
