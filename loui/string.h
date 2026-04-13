#pragma once

#include <string.h>

#include "carma_string.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct StringRange {
    char* data;
    size_t count;
} StringRange;

// Conversion for char arrays:
#define STRING_CAPACITY(a) (sizeof(a) / sizeof(char))
#define MAKE_STRING_RANGE(a) ((StringRange){(a), strlen((a))})
#define MAKE_STRING_BUILDER(a) ((StringBuilder){(a), strlen((a)), STRING_CAPACITY(a)})

#ifdef __cplusplus
}
#endif
