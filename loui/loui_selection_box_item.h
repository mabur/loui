#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LouiSelectionBoxItem {
    int x;
    int y;
    int width;
    int height;
    const char* text;
    bool is_selected;
    bool is_clicked;
} LouiSelectionBoxItem;

LouiSelectionBoxItem loui_update_selection_box_item(LouiSelectionBoxItem widget);

#ifdef __cplusplus
}
#endif
