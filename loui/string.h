#pragma once

#include "state.h"

#include <string.h>

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

#define MAKE_STRING_RANGE(s) ((StringRange){(s).data, strlen((s).data)})

#ifdef __cplusplus
}
#endif
