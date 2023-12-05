#ifndef RENDER_H_
#define RENDER_H_

#include "util.h"
#include "shm.h"
#include "user.h"
#include "wlr-layer-shell-unstable-v1-protocol.h"
#include "pango/pango-types.h"
#include <pango/pango.h>
#include <cairo.h>

struct Font {
    PangoFontDescription *description;
    unsigned int height, approx_width;
};

/* The render pipeline, also handles click events by keeping track of each components bounds'. */
struct Pipeline {
    struct List *callbacks; /* struct PipelineCallbacks* */
    int current /* The current callback we are on */,
        invalid;

    /* Colors */
    int background[4], foreground[4];

    PangoContext *context;
    struct Font *font;

    struct Shm *shm;
    struct wl_surface *surface;
    struct zwlr_layer_surface_v1 *layer_surface;
};

struct PipelineListener {
    void (*render)(struct Pipeline *pipeline, void *data, cairo_t *painter, int *x, int *y);
    int (*width)(struct Pipeline *pipeline, void *data, unsigned int future_widths);
};

struct PipelineCallback {
    const struct PipelineListener *listener;
    void *data;
};

/* Basic helper component, can be used if the only thing to be displayed is text. */
struct BasicComponent {
    PangoLayout *layout;
    int width, height,
        x, y /* box start coordinates */,
        tx, ty /* text starts coordinates, tx is added to starting x coordinates. */;
};

struct BasicComponent *basic_component_create(PangoContext *context, PangoFontDescription *description);
void basic_component_destroy(struct BasicComponent *component);
int basic_component_is_clicked(struct BasicComponent *component, double x, double y);
void basic_component_render(struct BasicComponent *component, struct Pipeline *pipeline,
        cairo_t *painter, int *x, int *y);
int basic_component_text_width(struct BasicComponent *component);
void pipeline_add(struct Pipeline *pipeline, const struct PipelineListener *listener, void *data);
struct Pipeline *pipeline_create(void);
void pipeline_destroy(struct Pipeline *pipeline);
int pipeline_get_future_widths(struct Pipeline *pipeline);
void pipeline_hide(struct Pipeline *pipeline);
void pipeline_invalidate(struct Pipeline *pipeline);
int pipeline_is_visible(struct Pipeline *pipeline);
void pipeline_show(struct Pipeline *pipeline, struct wl_output *output);
void pipeline_set_colorscheme(struct Pipeline* pipeline, const int **scheme);
void pipeline_color_foreground(struct Pipeline* pipeline, cairo_t *painter);
void pipeline_color_background(struct Pipeline* pipeline, cairo_t *painter);
void set_color(cairo_t *painter, const int rgba[4]);

#endif // RENDER_H_
