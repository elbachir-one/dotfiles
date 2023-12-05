#ifndef USER_H_
#define USER_H_

#include "main.h"

typedef struct Binding Binding;

enum Clicked {
    Click_None,
    Click_Tag,
    Click_Layout,
    Click_Title,
    Click_Status,
};

enum ColorScheme {
    InActive_Scheme = 0,
    Active_Scheme = 1,
    Urgent_Scheme = 2,
};

enum TouchGesture {
    Gesture_Left,
    Gesture_Right,
};

enum PointerScroll {
    Scroll_Up,
    Scroll_Down,
    Scroll_Left,
    Scroll_Right,
};

union Arg {
    unsigned int ui;
    int i;
    const void *v;
};

struct Binding {
    enum Clicked clicked;
    int button;
    void (*callback)(struct Monitor *monitor, const union Arg *arg);
    /* Informs the click function that they should only pass the defined arg in this binding */
    unsigned int bypass;
    const union Arg arg;
};

void layout(struct Monitor *monitor, const union Arg *arg);
void spawn(struct Monitor *monitor, const union Arg *arg);
void tag(struct Monitor *monitor, const union Arg *arg);
void toggle_view(struct Monitor *monitor, const union Arg *arg);
void view(struct Monitor *monitor, const union Arg *arg);

#endif // USER_H_
