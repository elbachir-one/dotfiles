#ifndef INPUT_H_
#define INPUT_H_

#include "main.h"
#include "util.h"
#include "user.h"
#include <wayland-util.h>
#include <wayland-cursor.h>

#define SCROLL_TIMEOUT 1000
#define SCROLL_THRESHOLD 10000

struct TouchPoint {
    int32_t id;
    uint32_t time;
    struct Monitor *focused_monitor;
    double start_x, start_y,
           x, y;
};

struct Touch {
    struct wl_touch *touch;
    struct TouchPoint points[16];
};

struct Axis {
    wl_fixed_t value;
    uint32_t discrete_steps, update_time;
};

struct Pointer {
    struct wl_pointer *pointer;
    struct Monitor *focused_monitor;

    struct wl_cursor_theme *cursor_theme;
    struct wl_cursor_image *cursor_image;
    struct wl_surface *cursor_surface;

    double x, y;
    struct List *buttons; /* uint32_t* */
    struct Axis axis[2];
    int scrolled;
};

struct Seat {
    uint32_t wl_name;
    struct wl_seat *seat;
    struct Pointer *pointer;
    struct Touch *touch;

    struct wl_list link;
};

struct HotspotListener {
    void (*click)(struct Monitor *monitor, void *data, uint32_t button, double x, double y);
    void (*bounds)(void *data, double *x, double *y, double *width, double *height);
};

struct Hotspot {
    const struct HotspotListener *listener;
    void *data;
};

extern const struct wl_seat_listener seat_listener;

void seat_destroy(struct Seat *seat);

#endif // INPUT_H_
