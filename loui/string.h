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

// Conversion for char arrays:
#define STRING_CAPACITY(a) (sizeof(a) / sizeof(char))
#define MAKE_STRING_RANGE(a) ((StringRange){(a), strlen((a))})
#define MAKE_STRING_BUILDER(a) ((StringBuilder){(a), strlen((a)), STRING_CAPACITY(a)})

#ifdef __cplusplus
}
#endif
