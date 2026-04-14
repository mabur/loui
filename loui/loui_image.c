#include "loui_image.h"

#include <carma/carma_parse.h>
#include <carma/carma_string.h>
#include "color.h"
#include "draw.h"
#include "rectangle.h"
#include "state.h"

LouiImage loui_update_image(LouiImage widget) {
    drawImage(s_loui.screen, widget.x, widget.y, widget.image.width, widget.image.height, widget.image.data);
    auto rectangle = (LouiRectangle){
        .x=widget.x, .y=widget.y, .width=widget.image.width, .height=widget.image.height,
    };
    widget.is_clicked = isLeftMouseButtonReleasedInside(rectangle);
    return widget;
}

ImageBuffer parse_transparent_ppm(const char* ppm_string, uint32_t rt, uint32_t gt, uint32_t bt) {
    auto s = STRING_VIEW(ppm_string);
    auto line0 = PARSE_LINE(s); // P3
    auto line1 = PARSE_LINE(s); // # Created by Paint Shop Pro 7 # CREATOR: GIMP PNM Filter Version 1.1
    auto width = PARSE_INT(s);
    PARSE_WHITESPACE(s);
    auto height = PARSE_INT(s);
    PARSE_WHITESPACE(s);
    auto max_intensity = PARSE_INT(s);
    auto image = (ImageBuffer){};
    INIT_2D_ARRAY(image, width, height);
    FOR_EACH(pixel, image) {
        PARSE_WHITESPACE(s);
        auto r = PARSE_INT(s);
        PARSE_WHITESPACE(s);
        auto g = PARSE_INT(s);
        PARSE_WHITESPACE(s);
        auto b = PARSE_INT(s);
        auto is_transparent = r == rt && g == gt && b==bt;
        auto color = is_transparent ? LOUI_TRANSPARENT_COLOR : LOUI_RGB(r, g, b);
        *pixel = color;
    }
    return image;
}
