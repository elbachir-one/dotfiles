#ifndef BAR_H_
#define BAR_H_

#include "config.h"
#include "render.h"

enum TagState {
  Tag_None   = 0,
  Tag_Active = 1,
  Tag_Urgent = 2,
};

struct Tag {
    unsigned int occupied, has_focused, state;
    struct BasicComponent *component;
};

struct Bar {
    struct Pipeline *pipeline;
    struct BasicComponent *layout, *title, *status;
    struct Tag tags[LENGTH(tags)];

    unsigned int active, floating;
    unsigned int x, y;
};

struct Bar *bar_create(struct List *hotspots, struct Pipeline *pipeline);
void bar_destroy(struct Bar *bar);
void bar_set_active(struct Bar *bar, unsigned int is_active);
void bar_set_floating(struct Bar *bar, unsigned int is_floating);
void bar_set_layout(struct Bar *bar, const char *text);
void bar_set_status(struct Bar *bar, const char *text);
void bar_set_tag(struct Bar *bar, unsigned int index,
        unsigned int state, unsigned int occupied, unsigned int focusedClient);
void bar_set_title(struct Bar *bar, const char *text);

extern const struct PipelineListener bar_pipeline_listener;

#endif // BAR_H_
