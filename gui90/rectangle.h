#pragma once

typedef struct Rectangle {
    int x;
    int y;
    int width;
    int height;
} Rectangle;

Rectangle shrinkRectangle(Rectangle rectangle);
