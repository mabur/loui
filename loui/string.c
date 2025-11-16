#include "string.h"

size_t getStringCapacity(String s) {
    return LOUI_MAX_SINGLE_LINE_TEXT_INPUT;
}

size_t getStringCount(String s) {
    return strlen(s.data);
}
