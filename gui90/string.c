#include "string.h"

int countRows(const char* text) {
    auto rows = 0;
    for (; *text; ++text) {
        if (*text == '\n') {
            ++rows;
        }
    }
    return rows;
}

static const char* gotoEndOfLine(const char* text) {
    for (;*text != '\0' || *text != '\n'; ++text) {
    }
    return text;
}

static const char* gotoNextLine(const char* text) {
    for (;; ++text) {
        if (*text == '\0') {
            return text;
        }
        if (*text == '\n') {
            ++text;
            return text;
        }
    }
}

static const char* gotoRow(const char* text, int row) {
    for (;row > 0; --row) {
        text = gotoNextLine(text);
    }
    return text;
}

static int countColumnsOfFirstRow(const char* text) {
    auto columns = 0;
    for (; *text && *text != '\n'; ++text, ++columns) {
    }
    return columns;
}

int countColumns(const char* text, int row) {
    return countColumnsOfFirstRow(gotoRow(text, row));
}
