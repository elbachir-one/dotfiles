#include "event.h"
#include "util.h"
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>

void events_add(struct Events *events, int fd, short mask, void *data,
        void (*callback)(int, short, void *)) {
    if (!events)
        return;

    if (events->pollfds_length == events->pollfds_capacity) {
        events->pollfds = realloc(events->pollfds, sizeof(struct pollfd) * (events->pollfds_capacity+1));
        events->pollfds_capacity++;
    }
    events->pollfds[events->pollfds_length++] = (struct pollfd){fd, mask, 0};

    struct EventCallback *backcall = list_add(events->callbacks, ecalloc(1, sizeof(*backcall)));
    backcall->callback = callback;
    backcall->data = data;
}

struct Events *events_create(void) {
    struct Events *events = ecalloc(1, sizeof(*events));

    events->callbacks = list_create(0);
    events->pollfds = NULL;
    events->pollfds_length = 0;
    events->pollfds_capacity = 0;

    return events;
}

void events_destroy(struct Events *events) {
    if (!events)
        return;

    list_elements_destroy(events->callbacks, free);
    free(events->pollfds);
    free(events);
}

void events_poll(struct Events *events) {
    if (!events)
        return;

    if (poll(events->pollfds, events->pollfds_length, -1) <= 0)
        return;

    for (int i = 0; i < events->pollfds_length; i++) {
        struct pollfd *pollfd = &events->pollfds[i];
        struct EventCallback *callback = events->callbacks->data[i];


        if (pollfd->revents & (pollfd->events | POLLHUP | POLLERR))
            callback->callback(pollfd->fd, pollfd->revents, callback->data);
    }
}

void events_remove(struct Events *events, int fd) {
    if (!events)
        return;

    for (int i = 0; i < events->pollfds_length; i++) {
        if (events->pollfds[i].fd != fd)
            continue;

        free(list_remove(events->callbacks, i));

        events->pollfds_length--;
        memmove(&events->pollfds[i], &events->pollfds[i+1],
                sizeof(struct pollfd) * (events->pollfds_length - i));
    }
}
