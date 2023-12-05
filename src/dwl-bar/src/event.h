#ifndef EVENT_H_
#define EVENT_H_
#include "util.h"

struct EventCallback {
    void (*callback)(int fd, short mask, void *data);
    void *data;
};

struct Events {
    struct List *callbacks; // struct EventCallback*
    struct pollfd *pollfds;
    int pollfds_length, pollfds_capacity;
};

void events_add(struct Events *events, int fd, short mask, void *data, void (*callback)(int fd, short mask, void *data));
struct Events *events_create(void);
void events_destroy(struct Events *events);
void events_poll(struct Events *events);
void events_remove(struct Events *events, int fd);

#endif // EVENT_H_
