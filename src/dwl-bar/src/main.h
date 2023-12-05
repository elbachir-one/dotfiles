#ifndef MAIN_H_
#define MAIN_H_

#include "dwl-ipc-unstable-v2-protocol.h"
#include <stdint.h>
#include <wayland-client.h>
#include <wayland-util.h>

#define VERSION 0.0

struct Monitor {
    uint32_t wl_name;

    unsigned int desired_visibility, tags, layout;
    struct wl_output *wl_output;
    struct zdwl_ipc_output_v2 *dwl_output;
    struct Pipeline *pipeline;
    struct List *hotspots; /* struct Hotspot* */
    struct Bar *bar;

    struct wl_list link;
};

void panic(const char *fmt, ...);
void monitors_update(void);
struct Monitor *monitor_from_surface(const struct wl_surface *surface);

extern struct wl_compositor *compositor;
extern struct zwlr_layer_shell_v1 *shell;
extern struct wl_shm *shm;
extern int layoutcount;

#endif // MAIN_H_
