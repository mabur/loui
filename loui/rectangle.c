#include "rectangle.h"

LouiRectangle shrinkRectangle(LouiRectangle rectangle) {
    rectangle.x += 1;
    rectangle.y += 1;
    rectangle.width -= 2;
    rectangle.height -= 2;
    return rectangle;
}
