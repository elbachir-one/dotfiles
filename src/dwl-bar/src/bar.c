#include "bar.h"
#include "cairo.h"
#include "config.h"
#include "input.h"
#include "main.h"
#include "pango/pango-item.h"
#include "pango/pango-layout.h"
#include "render.h"
#include "user.h"
#include "util.h"
#include "log.h"
#include "pango/pango.h"
#include <unistd.h>

static void bar_click(struct Monitor *monitor, void *data, uint32_t button, double x, double y);
static int bar_component_add_elipses(struct BasicComponent *component, struct Pipeline *pipeline, int limit);
static struct BasicComponent *bar_component_create(struct Pipeline *pipeline);
static int bar_component_width(struct BasicComponent *component, struct Pipeline *pipeline);
static void bar_bounds(void *data, double *x, double *y, double *width, double *height);
static enum Clicked bar_get_location(struct Bar *bar, double x, double y, int *tag_index);
static void bar_layout_render(struct Pipeline *pipeline, struct Bar *bar, cairo_t *painter, int *x, int *y);
static void bar_render(struct Pipeline *pipeline, void *data, cairo_t *painter, int *x, int *y);
static void bar_tags_render(struct Pipeline *pipeline, struct Bar *bar, cairo_t *painter, int *x, int *y);
static void bar_title_render(struct Pipeline *pipeline, struct Bar *bar, cairo_t *painter, int *x, int *y);
static void bar_status_render(struct Pipeline *pipeline, struct Bar *bar, cairo_t *painter, int *x, int *y);
static int bar_width(struct Pipeline *pipeline, void *data, unsigned int future_widths);

const struct PipelineListener bar_pipeline_listener = { .render = bar_render, .width = bar_width, };
const struct HotspotListener bar_hotspot_listener = { .click = bar_click, .bounds = bar_bounds };

void bar_click(struct Monitor *monitor, void *data, uint32_t button, double x, double y) {
    if (!monitor || !data)
        return;

    struct Bar *bar = data;
    const struct Binding *binding;
    union Arg *argp = NULL, arg;
    int tag_index = -1;
    enum Clicked clicked = bar_get_location(bar, x, y, &tag_index);

    if (clicked == Click_Tag) {
        arg.ui = tag_index;
        argp = &arg;
    }

    for (int i = 0; i < LENGTH(bindings); i++) {
        binding = &bindings[i];
        if (clicked != binding->clicked || button != binding->button)
            continue;

        binding->callback(monitor, (argp && !binding->bypass) ? argp : &binding->arg);
    }
}

int bar_component_add_elipses(struct BasicComponent *component, struct Pipeline *pipeline, int limit) {
    const char *current_string = pango_layout_get_text(component->layout);
    char *new_string;
    const int elipses_amnt = 3;
    int i;

    for (i = strlen(current_string);
            (((i+elipses_amnt)*pipeline->font->approx_width)+pipeline->font->height > limit && i >= 0);
            i--);

    if (i <= 0)
        return 0;

    new_string = strncpy(ecalloc(i+elipses_amnt+1, sizeof(*new_string)),
            current_string, i);
    new_string[i+1] = '\0';
    new_string = strcat(new_string, "...");

    pango_layout_set_text(component->layout, new_string, -1);
    free(new_string);
    return bar_component_width(component, pipeline);
}

struct BasicComponent *bar_component_create(struct Pipeline *pipeline) {
    if (!pipeline)
        return NULL;

    struct BasicComponent *component = basic_component_create(pipeline->context, pipeline->font->description);
    component->tx = pipeline->font->height/2.0;
    component->ty = 1;
    return component;
}

int bar_component_width(struct BasicComponent *component, struct Pipeline *pipeline) {
    return basic_component_text_width(component) + pipeline->font->height;
}

void bar_bounds(void *data, double *x, double *y, double *width, double *height) {
    struct Bar *bar = data;
    int bar_width = 0;
    struct Tag *tag;
    for (int i = 0; i < LENGTH(bar->tags); i++) {
        tag = &bar->tags[i];
        bar_width += tag->component->width;
    }
    bar_width += bar->layout->width;
    bar_width += bar->title->width;
    bar_width += bar->status->width;

    *x = bar->x;
    *y = bar->y;
    *width = bar_width;
    *height = bar->pipeline->shm->height;
}

enum Clicked bar_get_location(struct Bar *bar, double x, double y, int *tag_index) {
    enum Clicked clicked = Click_None;
    struct Tag *tag;

    for (int i = 0; i < LENGTH(bar->tags); i++) {
        tag = &bar->tags[i];

        if (!tag->occupied && !(tag->state & Tag_Active))
            continue;

        if (basic_component_is_clicked(tag->component, x, y)) {
            clicked = Click_Tag;
            *tag_index = i;
            return clicked;
        }
    }

    if (basic_component_is_clicked(bar->layout, x, y))
        clicked = Click_Layout;
    else if (basic_component_is_clicked(bar->title, x, y))
        clicked = Click_Title;
    else if (basic_component_is_clicked(bar->status, x, y))
        clicked = Click_Status;

    return clicked;
}

struct Bar *bar_create(struct List *hotspots, struct Pipeline *pipeline) {
    if (!pipeline)
        return NULL;

    struct Bar *bar = ecalloc(1, sizeof(*bar));
    bar->pipeline = pipeline;
    bar->title = bar_component_create(pipeline);
    bar->layout = bar_component_create(pipeline);
    bar->status = bar_component_create(pipeline);

    char *status = string_create("dwl %.1f", VERSION);
    pango_layout_set_text(bar->status->layout, status, strlen(status));
    free(status);

    struct Tag *tag;
    for (int i = 0; i < LENGTH(tags); i++) {
        tag = &bar->tags[i];
        *tag = (struct Tag){ 0, 0, 0,
            bar_component_create(pipeline) };
        pango_layout_set_text(tag->component->layout, tags[i], strlen(tags[i]));
        tag->component->width = basic_component_text_width(tag->component) + pipeline->font->height;
    }

    pipeline_add(pipeline, &bar_pipeline_listener, bar);
    struct Hotspot *hotspot = list_add(hotspots, ecalloc(1, sizeof(*hotspot)));
    hotspot->listener = &bar_hotspot_listener;
    hotspot->data = bar;

    bar->x = 0;
    bar->y = 0;

    return bar;
}

void bar_destroy(struct Bar *bar) {
    if (!bar) return;

    basic_component_destroy(bar->title);
    basic_component_destroy(bar->layout);
    basic_component_destroy(bar->status);
    struct Tag *tag;
    for (int i = 0; i < LENGTH(bar->tags); i++) {
        tag = &bar->tags[i];
        basic_component_destroy(tag->component);
    }
    free(bar);
}

void bar_layout_render(struct Pipeline *pipeline, struct Bar *bar, cairo_t *painter, int *x, int *y) {
    if (!bar || !pipeline)
        return;

    bar->layout->width = bar_component_width(bar->layout, pipeline);
    bar->layout->height = pipeline->shm->height;
    pipeline_set_colorscheme(pipeline, schemes[InActive_Scheme]);
    basic_component_render(bar->layout, pipeline, painter, x, y);

    *x += bar->layout->width;
}

void bar_render(struct Pipeline *pipeline, void *data, cairo_t *painter, int *x, int *y) {
    if (!pipeline || !data)
        return;

    struct Bar *bar = data;
    bar->x = *x;
    bar->y = *y;
    bar_tags_render(pipeline, bar, painter, x, y);
    bar_layout_render(pipeline, bar, painter, x, y);
    bar_title_render(pipeline, bar, painter, x, y);
    bar_status_render(pipeline, bar, painter, x, y);
}

void bar_tags_render(struct Pipeline *pipeline, struct Bar *bar, cairo_t *painter, int *x, int *y) {
    if (!bar || !pipeline)
        return;

    struct Tag *tag;
    for (int i = 0; i < LENGTH(bar->tags); i++) {
        tag = &bar->tags[i];

        if (!tag->occupied && !(tag->state & Tag_Active))
            continue;

        if (tag->state & Tag_Active)
            pipeline_set_colorscheme(pipeline, schemes[Active_Scheme]);
        else if (tag->state & Tag_Urgent)
            pipeline_set_colorscheme(pipeline, schemes[Urgent_Scheme]);
        else
            pipeline_set_colorscheme(pipeline, schemes[InActive_Scheme]);

        tag->component->height = pipeline->shm->height;
        basic_component_render(tag->component, pipeline, painter, x, y);

done:
        *x += tag->component->width;
    }
}

void bar_title_render(struct Pipeline *pipeline, struct Bar *bar, cairo_t *painter, int *x, int *y) {
    if (!bar || !pipeline)
        return;

    if (bar->active)
        pipeline_set_colorscheme(pipeline, schemes[Active_Scheme]);
    else
        pipeline_set_colorscheme(pipeline, schemes[InActive_Scheme]);

    bar->title->width = pipeline->shm->width - *x - bar_component_width(bar->status, pipeline) - pipeline_get_future_widths(pipeline);
    bar->title->height = pipeline->shm->height;

    if (bar_component_width(bar->title, pipeline) > bar->title->width)
        bar->title->width = bar_component_width(bar->title, pipeline);

    basic_component_render(bar->title, pipeline, painter, x, y);

    if (!bar->floating)
        goto done;

    int boxHeight = pipeline->font->height / 9,
        boxWidth  = pipeline->font->height / 6 + 1;

    set_color(painter, grey3);
    cairo_rectangle(painter, *x + boxHeight + 0.5, boxHeight + 0.5, boxWidth, boxWidth);
    cairo_set_line_width(painter, 1);
    cairo_stroke(painter);

done:
    *x += bar->title->width;
}

void bar_status_render(struct Pipeline *pipeline, struct Bar *bar, cairo_t *painter, int *x, int *y) {
    if (!bar || !pipeline)
        return;

    char *previous_status = NULL;

    pipeline_set_colorscheme(pipeline, schemes[InActive_Scheme]);
    if (!bar->active && status_on_active)
        pipeline_set_colorscheme(pipeline, (const int *[4]){ grey1, grey1 });

    bar->status->width = bar_component_width(bar->status, pipeline);
    bar->status->height = pipeline->shm->height;

    if (bar->status->width > (pipeline->shm->width - *x - pipeline_get_future_widths(pipeline))) {
        previous_status = strdup(pango_layout_get_text(bar->status->layout));
        bar->status->width = bar_component_add_elipses(bar->status, pipeline,
                (pipeline->shm->width - *x - pipeline_get_future_widths(pipeline)));
        if (bar->status->width == 0) {
            free(previous_status);
            return;
        }
    }

    basic_component_render(bar->status, pipeline, painter, x, y);

    if (previous_status) {
        pango_layout_set_text(bar->status->layout, previous_status, -1);
        free(previous_status);
    }

    *x += bar->status->width;
}

void bar_set_active(struct Bar *bar, unsigned int is_active) {
    if (!bar) return;

    bar->active = is_active;
}

void bar_set_floating(struct Bar *bar, unsigned int is_floating) {
    if (!bar) return;

    bar->floating = is_floating;
}

void bar_set_layout(struct Bar *bar, const char *text) {
    if (!bar) return;

    pango_layout_set_text(bar->layout->layout, text, -1);
}

void bar_set_status(struct Bar *bar, const char *text) {
    if (!bar) return;

    pango_layout_set_text(bar->status->layout, text, -1);
}

void bar_set_tag(struct Bar *bar, unsigned int index,
        unsigned int state, unsigned int occupied, unsigned int has_focused) {
    if (!bar) return;

    if (!bar || index >= LENGTH(bar->tags) ) return;

    struct Tag *tag = &bar->tags[index];
    tag->has_focused = has_focused;
    tag->occupied = occupied;
    tag->state = state;
}

void bar_set_title(struct Bar *bar, const char *text) {
    if (!bar) return;

    pango_layout_set_text(bar->title->layout, text, -1);
}

int bar_width(struct Pipeline *pipeline, void *data, unsigned int future_widths) {
    if (!data || !pipeline) return 0;

    struct Bar *bar = data;
    int width = 0, title_width, status_width;

    for (int i = 0; i < LENGTH(bar->tags); i++)
        width += bar_component_width(bar->tags[i].component, pipeline);
    width += bar_component_width(bar->layout, pipeline);

    title_width = pipeline->shm->width - width - bar_component_width(bar->status, pipeline) - future_widths;
    if (bar_component_width(bar->title, pipeline) > bar->title->width)
        title_width = bar_component_width(bar->title, pipeline);
    width += title_width;

    status_width = bar_component_width(bar->status, pipeline);
    if (status_width > (pipeline->shm->width - width - future_widths)) {
        char *previous_status = strdup(pango_layout_get_text(bar->status->layout));
        bar->status->width = bar_component_add_elipses(bar->status, pipeline,
                (pipeline->shm->width - width - pipeline_get_future_widths(pipeline)));
        pango_layout_set_text(bar->status->layout, previous_status, -1);
        free(previous_status);
    }
    width += status_width;

    return width;
}
