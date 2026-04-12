#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LouiSeparator {
    int x;
    int y;
    int width;
    int height;
} LouiSeparator;

LouiSeparator loui_update_separator(LouiSeparator widget);

#ifdef __cplusplus
}
#endif
