#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LouiSelectionBoxInit {
    int x;
    int y;
    int width;
    int height;
    bool is_clicked;
} LouiSelectionBoxInit;

LouiSelectionBoxInit loui_update_selection_box_init(LouiSelectionBoxInit widget);

#ifdef __cplusplus
}
#endif
