#pragma once

typedef struct LouiRectangle {
    int x;
    int y;
    int width;
    int height;
} LouiRectangle;

LouiRectangle shrinkRectangle(LouiRectangle rectangle);
