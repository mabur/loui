#pragma once

#include "color.h"

typedef struct LouiScreen {
    LouiColor* data;
    int width;
    int height;
    int count;
} LouiScreen;
