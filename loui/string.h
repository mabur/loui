#pragma once

#include "state.h"

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct StringRange {
    char* data;
    size_t count;
} StringRange;

typedef struct StringBuilder {
    char* data;
    size_t count;
    size_t capacity;
} StringBuilder;

#define MAKE_STRING_RANGE(s) ((StringRange){(s), strlen((s))})
#define MAKE_STRING_BUILDER(s) ((StringBuilder){(s), strlen((s)), LOUI_MAX_SINGLE_LINE_TEXT_INPUT})

#ifdef __cplusplus
}
#endif
