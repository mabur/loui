#include "loui_image.h"

#include "carma_parse.h"
#include "carma_string.h"
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
    auto line = (StringView){};
    parse_line(&s, &line); // P3
    parse_line(&s, &line); // # Created by Paint Shop Pro 7 # CREATOR: GIMP PNM Filter Version 1.1
    int width, height, max_intensity;
    parse_int(&s, &width);
    parse_whitespace(&s);
    parse_int(&s, &height);
    parse_whitespace(&s);
    parse_int(&s, &max_intensity);
    auto image = (ImageBuffer){};
    INIT_2D_ARRAY(image, width, height);
    FOR_EACH(pixel, image) {
        int r, g, b;
        parse_whitespace(&s);
        parse_int(&s, &r);
        parse_whitespace(&s);
        parse_int(&s, &g);
        parse_whitespace(&s);
        parse_int(&s, &b);
        auto is_transparent = r == rt && g == gt && b==bt;
        auto color = is_transparent ? LOUI_TRANSPARENT_COLOR : LOUI_RGB(r, g, b);
        *pixel = color;
    }
    return image;
}
