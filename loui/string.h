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

typedef struct StringBuilder {
    char* data;
    size_t count;
    size_t capacity;
} StringBuilder;

#define MAKE_STRING_RANGE(s) ((StringRange){(s).data, strlen((s).data)})
#define MAKE_STRING_BUILDER(s) ((StringBuilder){(s).data, strlen((s).data), LOUI_MAX_SINGLE_LINE_TEXT_INPUT})

#ifdef __cplusplus
}
#endif
