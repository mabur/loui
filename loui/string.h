#pragma once

#include "state.h"

typedef struct String {
    char data[LOUI_MAX_SINGLE_LINE_TEXT_INPUT];
} String;

size_t getStringCapacity(String s);
size_t getStringCount(String s);
