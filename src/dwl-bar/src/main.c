#include "bar.h"
#include "config.h"
#include "dwl-ipc-unstable-v2-protocol.h"
#include "event.h"
#include "log.h"
#include "render.h"
#include "util.h"
#include "main.h"
#include "input.h"
#include "xdg-shell-protocol.h"
#include "wlr-layer-shell-unstable-v1-protocol.h"
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <poll.h>
#include <sys/stat.h>
#include <wayland-client-core.h>
#include <wayland-client-protocol.h>
#include <wayland-client.h>
#include <wayland-util.h>

static void check_global(void *global, const char *name);
static void check_globals(void);
static void cleanup(void);
static void display_in(int fd, short mask, void *data);
static void fifo_handle(const char *line);
static void fifo_in(int fd, short mask, void *data);
static void fifo_setup(void);
static void monitor_destroy(struct Monitor *monitor);
struct Monitor *monitor_from_surface(const struct wl_surface *surface);
static void monitor_initialize(struct Monitor *monitor);
static void monitor_update(struct Monitor *monitor);
static void pipe_in(int fd, short mask, void *data);
static void registry_global_add(void *data, struct wl_registry *registry, uint32_t name,
                        const char *interface, uint32_t version);
static void registry_global_remove(void *data, struct wl_registry *registry, uint32_t name);
static void run(void);
static void set_cloexec(int fd);
static void setup(void);
static void sigaction_handler(int _);
static void xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial);
static void zdwl_ipc_manager_layout(void *data, struct zdwl_ipc_manager_v2 *zdwl_ipc_manager_v2, const char *name);
static void zdwl_ipc_manager_tags(void *data, struct zdwl_ipc_manager_v2 *zdwl_ipc_manager_v2, uint32_t amount);
static void zdwl_ipc_output_active(void *data, struct zdwl_ipc_output_v2 *zdwl_ipc_output_v2, uint32_t active);
static void zdwl_ipc_output_appid(void *data, struct zdwl_ipc_output_v2 *zdwl_ipc_output_v2, const char *appid);
static void zdwl_ipc_output_floating(void *data, struct zdwl_ipc_output_v2 *zdwl_ipc_output_v2, uint32_t is_floating);
static void zdwl_ipc_output_frame(void *data, struct zdwl_ipc_output_v2 *zdwl_ipc_output_v2);
static void zdwl_ipc_output_fullscreen(void *data, struct zdwl_ipc_output_v2 *zdwl_ipc_output_v2, uint32_t is_fullscreen);
static void zdwl_ipc_output_layout(void *data, struct zdwl_ipc_output_v2 *zdwl_ipc_output_v2, uint32_t layout);
static void zdwl_ipc_output_layout_symbol(void *data, struct zdwl_ipc_output_v2 *zdwl_ipc_output_v2, const char *layout);
static void zdwl_ipc_output_tag(void *data, struct zdwl_ipc_output_v2 *zdwl_ipc_output_v2,
        uint32_t tag, uint32_t state, uint32_t clients, uint32_t focused);
static void zdwl_ipc_output_title(void *data, struct zdwl_ipc_output_v2 *zdwl_ipc_output_v2, const char *title);
static void zdwl_ipc_output_toggle_visibility(void *data, struct zdwl_ipc_output_v2 *zdwl_ipc_output_v2);

static struct xdg_wm_base *base;
struct wl_compositor *compositor;
static struct wl_display *display;
static int display_fd;
static struct zdwl_ipc_manager_v2 *dwl_manager = NULL;
static struct Events *events;
static int fifo_fd;
static char *fifo_path;
int layoutcount;
static struct wl_list monitors; // struct Monitor*
static struct zxdg_output_manager_v1 *output_manager;
static const struct wl_registry_listener registry_listener = {
    .global = registry_global_add,
    .global_remove = registry_global_remove,
};
static int running = 0;
static struct wl_list seats; // struct Seat*
static int self_pipe[2];
struct zwlr_layer_shell_v1 *shell;
struct wl_shm *shm;
static int tagcount;
static const struct xdg_wm_base_listener xdg_wm_base_listener = {
    .ping = xdg_wm_base_ping,
};
static const struct zdwl_ipc_manager_v2_listener zdwl_manager_listener = {
    .layout = zdwl_ipc_manager_layout,
    .tags = zdwl_ipc_manager_tags,
};
static const struct zdwl_ipc_output_v2_listener zdwl_output_listener = {
    .active = zdwl_ipc_output_active,
    .appid = zdwl_ipc_output_appid,
    .floating = zdwl_ipc_output_floating,
    .frame = zdwl_ipc_output_frame,
    .fullscreen = zdwl_ipc_output_fullscreen,
    .layout = zdwl_ipc_output_layout,
    .layout_symbol = zdwl_ipc_output_layout_symbol,
    .tag = zdwl_ipc_output_tag,
    .title = zdwl_ipc_output_title,
    .toggle_visibility = zdwl_ipc_output_toggle_visibility,
};

void check_global(void *global, const char *name) {
    if (global)
        return;
    panic("Wayland compositor did not export: %s", name);
}

void check_globals(void) {
    check_global(base, "xdg_wm_base");
    check_global(compositor, "wl_compositor");
    check_global(dwl_manager, "zdwl_ipc_manager_v2");
    check_global(shell, "zwlr_layer_shell_v1");
    check_global(shm, "wl_shm");
}

void cleanup(void) {
    xdg_wm_base_destroy(base);
    wl_compositor_destroy(compositor);
    close(fifo_fd);
    unlink(fifo_path);
    free(fifo_path);
    zwlr_layer_shell_v1_destroy(shell);
    zdwl_ipc_manager_v2_destroy(dwl_manager);
    wl_shm_destroy(shm);
    events_destroy(events);
    log_destroy();

    struct Monitor *monitor, *tmp_monitor;
    wl_list_for_each_safe(monitor, tmp_monitor, &monitors, link)
        monitor_destroy(monitor);

    struct Seat *seat, *tmp_seat;
    wl_list_for_each_safe(seat, tmp_seat, &seats, link)
        seat_destroy(seat);

    wl_display_disconnect(display);
}

void display_in(int fd, short mask, void *data) {
    if (mask & (POLLHUP | POLLERR) ||
            wl_display_dispatch(display) == -1) {
        running = 0;
        return;
    }
}

void fifo_handle(const char *line) {
    char *command;
    unsigned long loc = 0;

    command = to_delimiter(line, &loc, ' ');

    if (STRING_EQUAL(command, "status")) {
        char *status = to_delimiter(line, &loc, '\n');
        struct Monitor *pos;
        wl_list_for_each(pos, &monitors, link) {
            bar_set_status(pos->bar, status);
            pipeline_invalidate(pos->pipeline);
        }
        free(status);
    }

    free(command);
}

void fifo_in(int fd, short mask, void *data) {
    if (mask & POLLERR) {
        events_remove(events, fd);
        char *default_status = string_create("dwl %.1f", VERSION);
        struct Monitor *pos;
        wl_list_for_each(pos, &monitors, link) {
            bar_set_status(pos->bar, default_status);
            pipeline_invalidate(pos->pipeline);
        }
        free(default_status);
        return;
    }

    int new_fd = dup(fd);
    FILE *fifo_file = fdopen(new_fd, "r");
    char *buffer = NULL;
    size_t size = 0;
    while (1) {
        if (getline(&buffer, &size, fifo_file) == -1)
            break;

        fifo_handle(buffer);
    }
    free(buffer);
    fclose(fifo_file);
    close(new_fd);

}

void fifo_setup(void) {
  int result, i;
  char *runtime_path = getenv("XDG_RUNTIME_DIR");

  for (i = 0; i < 100; i++) {
    fifo_path = string_create("%s/dwl-bar-%d", runtime_path, i);

    result = mkfifo(fifo_path, 0666);
    if (result < 0) {
      if (errno != EEXIST)
          panic("mkfifo");

      continue;
    }

    if ((fifo_fd = open(fifo_path, O_CLOEXEC | O_RDWR | O_NONBLOCK)) < 0)
        panic("open fifo");

    return;
  }

  panic("setup fifo"); /* If we get here then we couldn't setup the fifo */
}

void monitor_destroy(struct Monitor *monitor) {
    if (!monitor)
        return;

    if (wl_output_get_version(monitor->wl_output) >= WL_OUTPUT_RELEASE_SINCE_VERSION)
        wl_output_release(monitor->wl_output);
    wl_output_release(monitor->wl_output);
    zdwl_ipc_output_v2_destroy(monitor->dwl_output);
    list_elements_destroy(monitor->hotspots, free);
    pipeline_destroy(monitor->pipeline);
    bar_destroy(monitor->bar);
    free(monitor);
}

struct Monitor *monitor_from_surface(const struct wl_surface *surface) {
    struct Monitor *pos;
    wl_list_for_each(pos, &monitors, link) {
        if (pos->pipeline->surface == surface)
            return pos;
    }

    return NULL;
}

void monitor_initialize(struct Monitor *monitor) {
    if (!monitor) return;

    monitor->desired_visibility = show_bar;
    monitor->hotspots = list_create(1);
    monitor->pipeline = pipeline_create();
    monitor->bar = bar_create(monitor->hotspots, monitor->pipeline);
    if (!monitor->pipeline || !monitor->bar)
        panic("Failed to create a pipline or bar for a monitor");
    monitor_update(monitor);
}

void monitor_update(struct Monitor *monitor) {
    if (!monitor)
        return;

    if (!pipeline_is_visible(monitor->pipeline) && monitor->desired_visibility) {
        pipeline_show(monitor->pipeline, monitor->wl_output);
        return;
    }

    pipeline_invalidate(monitor->pipeline);
}

void monitors_update(void) {
    struct Monitor *monitor;
    wl_list_for_each(monitor, &monitors, link) {
        monitor_update(monitor);
    }
}

void pipe_in(int fd, short mask, void *data) {
    running = 0;
}

void registry_global_add(void *data, struct wl_registry *registry, uint32_t name,
                        const char *interface, uint32_t version) {
    if (STRING_EQUAL(interface, wl_compositor_interface.name))
        compositor = wl_registry_bind(registry, name, &wl_compositor_interface, 4);
    else if (STRING_EQUAL(interface, wl_output_interface.name)) {
        struct Monitor *monitor = ecalloc(1, sizeof(*monitor));
        monitor->wl_output = wl_registry_bind(registry, name, &wl_output_interface, 1);
        monitor->wl_name = name;
        monitor->dwl_output = NULL;

        wl_list_insert(&monitors, &monitor->link);

        if (!dwl_manager) return;

        monitor->dwl_output = zdwl_ipc_manager_v2_get_output(dwl_manager, monitor->wl_output);
        zdwl_ipc_output_v2_add_listener(monitor->dwl_output, &zdwl_output_listener, monitor);

        if (!running) return;
        monitor_initialize(monitor);
    }
    else if (STRING_EQUAL(interface, wl_seat_interface.name)) {
        struct Seat *seat = ecalloc(1, sizeof(*seat));
        seat->seat = wl_registry_bind(registry, name, &wl_seat_interface, 5);
        seat->wl_name = name;
        seat->pointer = NULL;
        seat->touch = NULL;
        wl_list_insert(&seats, &seat->link);
        wl_seat_add_listener(seat->seat, &seat_listener, seat);
    }
    else if (STRING_EQUAL(interface, wl_shm_interface.name))
        shm = wl_registry_bind(registry, name, &wl_shm_interface, 1);
    else if (STRING_EQUAL(interface, xdg_wm_base_interface.name)) {
        base = wl_registry_bind(registry, name, &xdg_wm_base_interface, 2);
        xdg_wm_base_add_listener(base, &xdg_wm_base_listener, NULL);
    }
    else if (STRING_EQUAL(interface, zdwl_ipc_manager_v2_interface.name)) {
        dwl_manager = wl_registry_bind(registry, name, &zdwl_ipc_manager_v2_interface, 2);
        zdwl_ipc_manager_v2_add_listener(dwl_manager, &zdwl_manager_listener, NULL);

        struct Monitor *monitor;
        wl_list_for_each(monitor, &monitors, link) {
            if (monitor->dwl_output) continue;

            monitor->dwl_output = zdwl_ipc_manager_v2_get_output(dwl_manager, monitor->wl_output);
            zdwl_ipc_output_v2_add_listener(monitor->dwl_output, &zdwl_output_listener, monitor);
        }
    }
    else if (STRING_EQUAL(interface, zwlr_layer_shell_v1_interface.name))
        shell = wl_registry_bind(registry, name, &zwlr_layer_shell_v1_interface, 4);
}

void registry_global_remove(void *data, struct wl_registry *registry, uint32_t name) {
    struct Monitor *monitor, *tmp_monitor;
    wl_list_for_each_safe(monitor, tmp_monitor, &monitors, link) {
        if (monitor->wl_name != name) continue;
        wl_list_remove(&monitor->link);
        monitor_destroy(monitor);
    }

    struct Seat *seat, *tmp_seat;
    wl_list_for_each_safe(seat, tmp_seat, &seats, link) {
        if (seat->wl_name != name) continue;
        wl_list_remove(&seat->link);
        seat_destroy(seat);
    }
}

void run(void) {
    running = 1;

    while (running) {
        wl_display_dispatch_pending(display);
        if (wl_display_flush(display) == -1 && errno != EAGAIN)
            break;

        events_poll(events);
    }
}

void set_cloexec(int fd) {
    int flags = fcntl(fd, F_GETFD);
    if (flags == -1)
        panic("F_GETFD");
    if (fcntl(fd, F_SETFD, flags | FD_CLOEXEC) < 0)
        panic("FD_SETFD");
}

void setup(void) {
    if (pipe(self_pipe) == -1)
        panic("pipe");

    set_cloexec(self_pipe[0]);
    set_cloexec(self_pipe[1]);

    static struct sigaction sighandle;
    static struct sigaction child_sigaction;

    sighandle.sa_handler = &sigaction_handler;
    child_sigaction.sa_handler = SIG_IGN;

    if (sigaction(SIGTERM, &sighandle, NULL) < 0)
        panic("sigaction SIGTERM");
    if (sigaction(SIGINT, &sighandle, NULL) < 0)
        panic("sigaction SIGINT");
    if (sigaction(SIGCHLD, &child_sigaction, NULL) < 0)
        panic("sigaction SIGCHLD");

    display = wl_display_connect(NULL);
    if (!display)
        panic("Failed to connect to Wayland compositor.");
    display_fd = wl_display_get_fd(display);

    wl_list_init(&seats);
    wl_list_init(&monitors);

    struct wl_registry *registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, NULL);
    wl_display_roundtrip(display);

    fifo_setup();

    check_globals();

    wl_display_roundtrip(display);

    if (tagcount != LENGTH(tags))
        panic("We do not have the same amount of tags as dwl! Please check config.def.h!");

    struct Monitor *monitor;
    wl_list_for_each(monitor, &monitors, link) {
        monitor_initialize(monitor);
    }

    if (fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK) < 0)
        panic("STDIN_FILENO O_NONBLOCK");

    events = events_create();
    events_add(events, display_fd, POLLIN, NULL, display_in);
    events_add(events, self_pipe[0], POLLIN, NULL, pipe_in);
    events_add(events, fifo_fd, POLLIN, NULL, fifo_in);
}

void sigaction_handler(int _) {
    if (write(self_pipe[1], "0", 1) < 0)
        panic("sigaction_handler");
}

void xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial) {
    xdg_wm_base_pong(xdg_wm_base, serial);
}

void zdwl_ipc_manager_layout(void *data, struct zdwl_ipc_manager_v2 *zdwl_ipc_manager_v2, const char *name) {
    layoutcount++;
}

void zdwl_ipc_manager_tags(void *data, struct zdwl_ipc_manager_v2 *zdwl_ipc_manager_v2, uint32_t amount) {
    tagcount = amount;
}

void zdwl_ipc_output_active(void *data, struct zdwl_ipc_output_v2 *zdwl_ipc_output_v2, uint32_t active) {
    struct Monitor *monitor = data;
    bar_set_active(monitor->bar, active);
}

void zdwl_ipc_output_appid(void *data, struct zdwl_ipc_output_v2 *zdwl_ipc_output_v2, const char *appid) {
    /* Nop */
}

void zdwl_ipc_output_floating(void *data, struct zdwl_ipc_output_v2 *zdwl_ipc_output_v2, uint32_t is_floating) {
    struct Monitor *monitor = data;
    bar_set_floating(monitor->bar, is_floating);
}

void zdwl_ipc_output_frame(void *data, struct zdwl_ipc_output_v2 *zdwl_ipc_output_v2) {
    struct Monitor *monitor = data;
    monitor_update(monitor);
}

void zdwl_ipc_output_fullscreen(void *data, struct zdwl_ipc_output_v2 *zdwl_ipc_output_v2, uint32_t is_fullscreen) {
    /* Nop */
}

void zdwl_ipc_output_layout(void *data, struct zdwl_ipc_output_v2 *zdwl_ipc_output_v2, uint32_t layout) {
    struct Monitor *monitor = data;
    monitor->layout = layout;
}

void zdwl_ipc_output_layout_symbol(void *data, struct zdwl_ipc_output_v2 *zdwl_ipc_output_v2, const char *layout) {
    struct Monitor *monitor = data;
    bar_set_layout(monitor->bar, layout);
}

void zdwl_ipc_output_tag(void *data, struct zdwl_ipc_output_v2 *zdwl_ipc_output_v2, uint32_t tag, uint32_t state, uint32_t clients, uint32_t focused) {
    struct Monitor *monitor = data;
    bar_set_tag(monitor->bar, tag, state, clients ? 1 : 0, focused);
    monitor->tags = (state & ZDWL_IPC_OUTPUT_V2_TAG_STATE_ACTIVE) ? monitor->tags | (1 << tag) : monitor->tags & ~(1 << tag);
}

void zdwl_ipc_output_title(void *data, struct zdwl_ipc_output_v2 *zdwl_ipc_output_v2, const char *title) {
    struct Monitor *monitor = data;
    bar_set_title(monitor->bar, title);
}

void zdwl_ipc_output_toggle_visibility(void *data, struct zdwl_ipc_output_v2 *zdwl_ipc_output_v2) {
    struct Monitor *monitor = data;
    monitor->desired_visibility ^= 1;
    pipeline_hide(monitor->pipeline);
    monitor_update(monitor);
}

int main(int argc, char *argv[]) {
    int opt;
    while((opt = getopt(argc, argv, "hlv")) != -1) {
        switch (opt) {
            case 'l':
                if (!setup_log())
                    panic("Failed to setup logging");
                break;
            case 'h':
                printf("Usage: %s [-h] [-v]\n", argv[0]);
                exit(EXIT_SUCCESS);
            case 'v':
                printf("%s %.1f\n", argv[0], VERSION);
                exit(EXIT_SUCCESS);
            case '?':
                printf("Invalid Argument\n");
                printf("Usage: %s [-h] [-v] [-l]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    setup();
    run();
    cleanup();
}

void panic(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "[dwl-bar] panic: ");
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    if (fmt[0] && fmt[strlen(fmt) - 1] == ':') {
        fputc(' ', stderr);
        perror(NULL);

    } else {
        fputc('\n', stderr);
    }

    cleanup();
    exit(EXIT_FAILURE);
}
