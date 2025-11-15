#pragma once

#include "state.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct String {
    char data[LOUI_MAX_SINGLE_LINE_TEXT_INPUT];
} String;

typedef struct StringRange {
    char* data;
    size_t count;
} StringRange;

size_t getStringCapacity(String s);
size_t getStringCount(String s);

#ifdef __cplusplus
}
#endif
