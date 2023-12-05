#include "input.h"
#include "log.h"
#include "main.h"
#include "user.h"
#include "util.h"
#include "render.h"
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <linux/input-event-codes.h>
#include <wayland-client-protocol.h>
#include <wayland-cursor.h>
#include <wayland-util.h>

static int button_cmp(const void *left, const void *right);
static void hotspots_process(struct Monitor* monitor, double x, double y, uint32_t button);
static void pointer_axis(void *data, struct wl_pointer *wl_pointer, uint32_t time, uint32_t axis, wl_fixed_t value);
static void pointer_axis_discrete(void *data, struct wl_pointer *wl_pointer, uint32_t axis, int32_t discrete);
static void pointer_axis_source(void *data, struct wl_pointer *wl_pointer, uint32_t axis_source);
static void pointer_axis_stop(void *data, struct wl_pointer *wl_pointer, uint32_t time, uint32_t axis);
static void pointer_button(void *data, struct wl_pointer *wl_pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state);
static struct Pointer *pointer_create(struct wl_seat *seat);
static void pointer_destroy(struct Pointer *pointer);
static void pointer_enter(void *data, struct wl_pointer *wl_pointer, uint32_t serial, struct wl_surface *surface, wl_fixed_t surface_x, wl_fixed_t surface_y);
static void pointer_frame(void *data, struct wl_pointer *wl_pointer);
static void pointer_process_scroll(struct Pointer *pointer, unsigned int axis_index);
static void pointer_leave(void *data, struct wl_pointer *wl_pointer, uint32_t serial, struct wl_surface *surface);
static void pointer_motion(void *data, struct wl_pointer *wl_pointer, uint32_t time, wl_fixed_t surface_x, wl_fixed_t surface_y);
static void pointer_update_cursor(struct Pointer *pointer);
static void seat_capabilities(void *data, struct wl_seat *wl_seat, uint32_t capabilities);
static void seat_name(void *data, struct wl_seat *wl_seat, const char *name);
static void touch_cancel(void *data, struct wl_touch *wl_touch);
static struct Touch *touch_create(struct wl_seat *seat);
static void touch_destroy(struct Touch *touch);
static void touch_down(void *data, struct wl_touch *wl_touch, uint32_t serial, uint32_t time, struct wl_surface *surface, int32_t id, wl_fixed_t x, wl_fixed_t y);
static void touch_frame(void *data, struct wl_touch *wl_touch);
static struct TouchPoint *touch_get_point(struct Touch *touch, int32_t id);
static void touch_motion(void *data, struct wl_touch *wl_touch, uint32_t time, int32_t id, wl_fixed_t x, wl_fixed_t y);
static void touch_orientation(void *data, struct wl_touch *wl_touch, int32_t id, wl_fixed_t orientation);
static uint32_t touch_point_to_button(struct TouchPoint *point, uint32_t time);
static void touch_shape(void *data, struct wl_touch *wl_touch, int32_t id, wl_fixed_t major, wl_fixed_t minor);
static void touch_up(void *data, struct wl_touch *wl_touch, uint32_t serial, uint32_t time, int32_t id);
static uint32_t wl_axis_to_button(int axis, wl_fixed_t value);

static const struct wl_pointer_listener pointer_listener = {
    .axis = pointer_axis,
    .axis_discrete = pointer_axis_discrete,
    .axis_source = pointer_axis_source,
    .axis_stop = pointer_axis_stop,
    .button = pointer_button,
    .enter = pointer_enter,
    .frame = pointer_frame,
    .leave = pointer_leave,
    .motion = pointer_motion,
};

const struct wl_seat_listener seat_listener = {
    .capabilities = seat_capabilities,
    .name = seat_name,
};

static const struct wl_touch_listener touch_listener = {
    .cancel = touch_cancel,
    .down = touch_down,
    .frame = touch_frame,
    .motion = touch_motion,
    .orientation = touch_orientation,
    .shape = touch_shape,
    .up = touch_up,
};

int button_cmp(const void *left, const void *right) {
    return (*(uint32_t*)left) == (*(uint32_t*)right);
}

void pointer_axis(void *data, struct wl_pointer *wl_pointer, uint32_t time, uint32_t axis_index, wl_fixed_t value) {
    struct Pointer *pointer = data;
    struct Axis *axis = &pointer->axis[axis_index];

    if (axis->discrete_steps == 0
            && time - axis->update_time - SCROLL_TIMEOUT)
        axis->value = 0;

    axis->value += value;
    axis->update_time = time;
    pointer->scrolled = 1;
}

void pointer_axis_discrete(void *data, struct wl_pointer *wl_pointer, uint32_t axis, int32_t discrete) {
    struct Pointer *pointer = data;

    pointer->axis[axis].discrete_steps += abs(discrete);
    pointer->scrolled = 1;
}

void pointer_axis_source(void *data, struct wl_pointer *wl_pointer, uint32_t axis_source) {
    /* Nop */
}

void pointer_axis_stop(void *data, struct wl_pointer *wl_pointer, uint32_t time, uint32_t axis) {
    /* Nop */
}

void pointer_button(void *data, struct wl_pointer *wl_pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state) {
    struct Pointer *pointer = data;
    int index;

    if (state == WL_POINTER_BUTTON_STATE_PRESSED && list_cmp_find(pointer->buttons, &button, button_cmp) == -1) {
        uint32_t *btn = list_add(pointer->buttons, ecalloc(1, sizeof(*btn)));
        *btn = button;
    }
    else if (state == WL_POINTER_BUTTON_STATE_RELEASED &&
            (index = list_cmp_find(pointer->buttons, &button, button_cmp)) != -1) {
        free(list_remove(pointer->buttons, index));
    }
}

struct Pointer *pointer_create(struct wl_seat *seat) {
    if (!seat) return NULL;

    struct Pointer *pointer = ecalloc(1, sizeof(*pointer));
    pointer->pointer = wl_seat_get_pointer(seat);
    pointer->scrolled = 0;
    pointer->buttons = list_create(0);
    pointer->focused_monitor = NULL;
    pointer->cursor_surface = NULL;
    pointer->cursor_image = NULL;
    pointer->cursor_theme = NULL;

    return pointer;
}

void pointer_destroy(struct Pointer *pointer) {
    if (!pointer) return;

    wl_pointer_release(pointer->pointer);
    wl_surface_destroy(pointer->cursor_surface);
    wl_cursor_theme_destroy(pointer->cursor_theme);
    list_elements_destroy(pointer->buttons, free);
    free(pointer);
}

void pointer_enter(void *data, struct wl_pointer *wl_pointer, uint32_t serial, struct wl_surface *surface, wl_fixed_t surface_x, wl_fixed_t surface_y) {
    struct Pointer *pointer = data;
    pointer->focused_monitor = monitor_from_surface(surface);
    if (!pointer->focused_monitor)
        return;

    pointer_update_cursor(pointer);
    wl_pointer_set_cursor(wl_pointer, serial, pointer->cursor_surface,
                        pointer->cursor_image->hotspot_x, pointer->cursor_image->hotspot_y);
}

void pointer_frame(void *data, struct wl_pointer *wl_pointer) {
    struct Pointer *pointer = data;
    struct Monitor *monitor = pointer->focused_monitor;
    if (!monitor) return;

    for (int i = 0; i < pointer->buttons->length; i++)
        hotspots_process(pointer->focused_monitor, pointer->x, pointer->y,
                *(uint32_t*)pointer->buttons->data[i]);
    list_elements_destroy(pointer->buttons, free);
    pointer->buttons = list_create(0);

    if (pointer->scrolled) {
        for (int i = 0; i < 2; i++)
            pointer_process_scroll(pointer, i);
    }
}

void pointer_process_scroll(struct Pointer *pointer, unsigned int axis_index) {
    struct Axis *axis = &pointer->axis[axis_index];
    if (axis->discrete_steps) {
        for (int i = 0; i < axis->discrete_steps; i++)
            hotspots_process(pointer->focused_monitor, pointer->x, pointer->y, wl_axis_to_button(axis_index, axis->value));
        axis->value = 0;
        axis->discrete_steps = 0;
    } else {
        while (abs(axis->value) > SCROLL_THRESHOLD) {
            if (axis->value > 0){
                hotspots_process(pointer->focused_monitor, pointer->x, pointer->y, wl_axis_to_button(axis_index, SCROLL_THRESHOLD));
                axis->value -= SCROLL_THRESHOLD;
            } else {
                hotspots_process(pointer->focused_monitor, pointer->x, pointer->y, wl_axis_to_button(axis_index, -SCROLL_THRESHOLD));
                axis->value += SCROLL_THRESHOLD;
            }
        }
    }
}

void pointer_leave(void *data, struct wl_pointer *wl_pointer, uint32_t serial, struct wl_surface *surface) {
    struct Pointer *pointer = data;
    pointer->focused_monitor = NULL;
}

void pointer_motion(void *data, struct wl_pointer *wl_pointer, uint32_t time, wl_fixed_t surface_x, wl_fixed_t surface_y) {
    struct Pointer *pointer = data;
    pointer->x = wl_fixed_to_double(surface_x);
    pointer->y = wl_fixed_to_double(surface_y);
}

void pointer_update_cursor(struct Pointer *pointer) {
    if (!pointer)
        return;

    if (!pointer->cursor_surface)
        pointer->cursor_surface = wl_compositor_create_surface(compositor);

    if (pointer->cursor_theme)
        wl_cursor_theme_destroy(pointer->cursor_theme);

    unsigned int cursor_size = 24;
    const char *cursor_theme = getenv("XCURSOR_THEME");
    const char *env_cursor_size = getenv("XCURSOR_SIZE");
    if (env_cursor_size && strlen(env_cursor_size) > 0) {
        errno = 0;
        char *end;
        unsigned int size = strtoul(env_cursor_size, &end, 10);
        if (!*end && errno == 0)
            cursor_size = size;
    }
    pointer->cursor_theme = wl_cursor_theme_load(cursor_theme, cursor_size, shm);
    pointer->cursor_image = wl_cursor_theme_get_cursor(pointer->cursor_theme, "left_ptr")->images[0];
    wl_surface_attach(pointer->cursor_surface, wl_cursor_image_get_buffer(pointer->cursor_image), 0, 0);
    wl_surface_commit(pointer->cursor_surface);
}

void hotspots_process(struct Monitor* monitor, double x, double y, uint32_t button) {
    struct Hotspot *hotspot;
    for (int i = 0; i < monitor->hotspots->length; i++) {
        hotspot = monitor->hotspots->data[i];

        double hotspot_x = 0, hotspot_y = 0, hotspot_width = 0, hotspot_height = 0;
        hotspot->listener->bounds(hotspot->data, &hotspot_x, &hotspot_y, &hotspot_width, &hotspot_height);

        if (!( x > hotspot_x && y > hotspot_y &&
                    x < (hotspot_x+hotspot_width) && y < (hotspot_y+hotspot_height)))
            continue;

        hotspot->listener->click(monitor, hotspot->data, button, x, y);
        return;
    }
}

void seat_capabilities(void *data, struct wl_seat *wl_seat, uint32_t capabilities) {
    struct Seat *seat = data;
    int has_pointer = capabilities & WL_SEAT_CAPABILITY_POINTER,
        has_touch = capabilities & WL_SEAT_CAPABILITY_TOUCH;

    if (!seat->pointer && has_pointer) {
        seat->pointer = pointer_create(seat->seat);
        wl_pointer_add_listener(seat->pointer->pointer, &pointer_listener, seat->pointer);
    }
    else if (seat->pointer && !has_pointer) {
        pointer_destroy(seat->pointer);
    }

    if (!seat->touch && has_touch) {
        seat->touch = touch_create(seat->seat);
        wl_touch_add_listener(seat->touch->touch, &touch_listener, seat->touch);
    }
    else if (seat->touch && !has_touch) {
        touch_destroy(seat->touch);
    }
}

void seat_destroy(struct Seat *seat) {
    if (!seat) return;

    pointer_destroy(seat->pointer);
    touch_destroy(seat->touch);
    wl_seat_release(seat->seat);
    free(seat);
}

void seat_name(void *data, struct wl_seat *wl_seat, const char *name) {
    /* Nop */
}

void touch_cancel(void *data, struct wl_touch *wl_touch) {
    struct Touch *touch = data;
    struct TouchPoint *point;
    for (int i = 0; i < LENGTH(touch->points); i++) {
        point = &touch->points[i];
        point->id = -1;
        point->focused_monitor = NULL;
    }
}

struct Touch *touch_create(struct wl_seat *seat) {
    if (!seat) return NULL;

    struct Touch *touch = ecalloc(1, sizeof(*touch));
    touch->touch = wl_seat_get_touch(seat);
    struct TouchPoint *point;
    for (int i = 0; i < LENGTH(touch->points); i++) {
        point = &touch->points[i];
        point->id = -1;
        point->focused_monitor = NULL;
    }

    return touch;
}

void touch_destroy(struct Touch *touch) {
    if (!touch) return;

    wl_touch_release(touch->touch);
    free(touch);
}

void touch_down(void *data, struct wl_touch *wl_touch, uint32_t serial, uint32_t time,
        struct wl_surface *surface, int32_t id, wl_fixed_t x, wl_fixed_t y) {
    struct Touch *touch =  data;
    struct TouchPoint *point = touch_get_point(touch, id);
    if (!point)
        return;

    point->focused_monitor = monitor_from_surface(surface);
    if (!point->focused_monitor)
        return;

    point->id = id;
    point->time = time;
    point->start_x = wl_fixed_to_double(x);
    point->start_y = wl_fixed_to_double(y);
}

void touch_frame(void *data, struct wl_touch *wl_touch) {
    /* Nop */
}

struct TouchPoint *touch_get_point(struct Touch *touch, int32_t id) {
    struct TouchPoint *point;
    int empty_index = -1;
    for (int i = 0; i < LENGTH(touch->points); i++) {
        point = &touch->points[i];
        if (point->id == id) {
            return point;
        }
        if (!point->focused_monitor && point->id == -1)
            empty_index = i;
    }

    if (empty_index == -1)
        return NULL;

    return &touch->points[empty_index];
}

void touch_motion(void *data, struct wl_touch *wl_touch, uint32_t time, int32_t id, wl_fixed_t x, wl_fixed_t y) {
    struct Touch *touch = data;
    struct TouchPoint *point = touch_get_point(touch, id);
    if (!point)
        return;

    point->time = time;
    point->x = wl_fixed_to_double(x);
    point->y = wl_fixed_to_double(y);
}

void touch_orientation(void *data, struct wl_touch *wl_touch, int32_t id, wl_fixed_t orientation) {
    /* Nop */
}

uint32_t touch_point_to_button(struct TouchPoint *point, uint32_t time) {
    /* "progress" is a measure from 0..100 representing the fraction of the
     * output the touch gesture has travelled, positive when moving to the right
     * and negative when moving to the left. */
    int progress = (int)((point->x - point->start_x) / point->focused_monitor->pipeline->shm->width * 100);
    if (abs(progress) > 20)
        return (progress > 0 ? Gesture_Right : Gesture_Left);

    if (time - point->time < 500)
        return BTN_LEFT;
    if (time - point->time < 1000)
        return BTN_RIGHT;

    /* If hold time is longer than 1 second then it is a middle click. */
    return BTN_MIDDLE;
}

void touch_shape(void *data, struct wl_touch *wl_touch, int32_t id, wl_fixed_t major, wl_fixed_t minor) {
    /* Nop */
}

void touch_up(void *data, struct wl_touch *wl_touch, uint32_t serial, uint32_t time, int32_t id) {
    struct Touch *touch = data;
    struct TouchPoint *point = touch_get_point(touch, id);
    if (!point) return;

    uint32_t button = touch_point_to_button(point, time);
    hotspots_process(point->focused_monitor, point->x, point->y, button);
}

uint32_t wl_axis_to_button(int axis, wl_fixed_t value) {
    int negative = wl_fixed_to_double(value) < 0;
    switch (axis) {
        case WL_POINTER_AXIS_VERTICAL_SCROLL:
            return negative ? Scroll_Up : Scroll_Down;
        case WL_POINTER_AXIS_HORIZONTAL_SCROLL:
            return negative ? Scroll_Left : Scroll_Right;
        default:
            return 0;
    }
}
