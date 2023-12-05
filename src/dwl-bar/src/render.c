#include "render.h"
#include "log.h"
#include "main.h"
#include "shm.h"
#include "util.h"
#include "config.h"
#include "wlr-layer-shell-unstable-v1-protocol.h"
#include "pango/pango-layout.h"
#include "pango/pangocairo.h"
#include <limits.h>
#include <stdlib.h>
#include <wayland-client-protocol.h>
#include <cairo.h>

static struct Font *get_font(void);
static void pipeline_frame(void* data, struct wl_callback* callback, uint32_t callback_data);
static void pipeline_layer_surface(void* data, struct zwlr_layer_surface_v1* _, uint32_t serial, uint32_t width, uint32_t height);
static void pipeline_render(struct Pipeline *pipeline);

const struct wl_callback_listener frame_listener = {.done = pipeline_frame};
const struct zwlr_layer_surface_v1_listener layer_surface_listener = {.configure = pipeline_layer_surface};

struct BasicComponent *basic_component_create(PangoContext *context, PangoFontDescription *description) {
    struct BasicComponent *component = ecalloc(1, sizeof(*component));
    component->layout = pango_layout_new(context);
    pango_layout_set_font_description(component->layout, description);
    component->x = 0;
    component->y = 0;
    component->width = 0;
    component->height = 0;
    component->tx = 0;
    component->ty = 0;

    return component;
}

void basic_component_destroy(struct BasicComponent *component) {
    if (!component)
        return;

    g_object_unref(component->layout);
    free(component);
}

int basic_component_is_clicked(struct BasicComponent *component, double x, double y) {
    return (x > component->x && y > component->y &&
            x < (component->x + component->width) && y < (component->y + component->height));
}

void basic_component_render(struct BasicComponent *component, struct Pipeline *pipeline,
        cairo_t *painter, int *x, int *y) {
    if (!component)
        return;

    pango_cairo_update_layout(painter, component->layout);
    component->x = *x;
    component->y = *y;

    pipeline_color_background(pipeline, painter);
    cairo_rectangle(painter, *x, *y, component->width, component->height);
    cairo_fill(painter);

    pipeline_color_foreground(pipeline, painter);
    cairo_move_to(painter, *x+component->tx, *y+component->ty);
    pango_cairo_show_layout(painter, component->layout);
}

int basic_component_text_width(struct BasicComponent *component) {
    if (!component)
        return 0;

    int w;
    pango_layout_get_size(component->layout, &w, NULL);
    return PANGO_PIXELS(w);
}

struct Font *get_font(void) {
    PangoFontMap* map = pango_cairo_font_map_get_default();
    if (!map)
        panic("font map");

    PangoFontDescription* desc = pango_font_description_from_string(font);
    if (!desc)
        panic("font description");

    PangoContext* context = pango_font_map_create_context(map);
    if (!context)
        panic("temp context");

    PangoFont* fnt = pango_font_map_load_font(map, context, desc);
    if (!fnt)
        panic("font load");

    PangoFontMetrics* metrics = pango_font_get_metrics(fnt, pango_language_get_default());
    if (!metrics)
        panic("font metrics");

    struct Font *font = ecalloc(1, sizeof(*font));
    font->description = desc;
    font->height = PANGO_PIXELS(pango_font_metrics_get_height(metrics));
    font->approx_width = PANGO_PIXELS(pango_font_metrics_get_approximate_char_width(metrics));

    pango_font_metrics_unref(metrics);
    g_object_unref(fnt);
    g_object_unref(context);

    return font;
}

void pipeline_add(struct Pipeline *pipeline, const struct PipelineListener *listener, void *data) {
    if (!pipeline)
        return;

    struct PipelineCallback *callback = list_add(pipeline->callbacks, ecalloc(1, sizeof(*callback)));
    callback->listener = listener;
    callback->data = data;
}

struct Pipeline *pipeline_create(void) {
    struct Pipeline *pipeline = ecalloc(1, sizeof(*pipeline));
    pipeline->callbacks = list_create(0);
    pipeline->current = 0;
    pipeline->invalid = 0;
    pipeline->context = pango_font_map_create_context(pango_cairo_font_map_get_default());
    pipeline->font = get_font();
    pipeline->shm = NULL;

    return pipeline;
}

void pipeline_destroy(struct Pipeline *pipeline) {
    if (!pipeline)
        return;

    list_elements_destroy(pipeline->callbacks, free);
    g_object_unref(pipeline->context);
    pango_font_description_free(pipeline->font->description);
    free(pipeline->font);
    shm_destroy(pipeline->shm);
    wl_surface_destroy(pipeline->surface);
    zwlr_layer_surface_v1_destroy(pipeline->layer_surface);
    free(pipeline);
}

void pipeline_frame(void* data, struct wl_callback* callback, uint32_t callback_data) {
    pipeline_render((struct Pipeline *)data);
    wl_callback_destroy(callback);
}

int pipeline_get_future_widths(struct Pipeline *pipeline) {
    if (!pipeline)
        return 0;

    int width = 0;
    struct PipelineCallback *callback;
    for (int i = pipeline->callbacks->length-1; i > pipeline->current; i--) {
        callback = pipeline->callbacks->data[i];
        width += callback->listener->width(pipeline, callback->data, width);
    }

    return width;
}

void pipeline_hide(struct Pipeline *pipeline) {
    if (!pipeline || !pipeline_is_visible(pipeline))
        return;


    zwlr_layer_surface_v1_destroy(pipeline->layer_surface);
    wl_surface_destroy(pipeline->surface);
    shm_destroy(pipeline->shm);

    pipeline->layer_surface = NULL;
    pipeline->surface = NULL;
    pipeline->shm = NULL;
}

void pipeline_invalidate(struct Pipeline *pipeline) {
    if (!pipeline || pipeline->invalid || !pipeline_is_visible(pipeline))
        return;

    struct wl_callback *callback = wl_surface_frame(pipeline->surface);
    wl_callback_add_listener(callback, &frame_listener, pipeline);
    wl_surface_commit(pipeline->surface);
    pipeline->invalid = 1;
}

int pipeline_is_visible(struct Pipeline *pipeline) {
    if (!pipeline) return 0;
    return !(!pipeline->surface);
}

void pipeline_layer_surface(void* data, struct zwlr_layer_surface_v1* _,
        uint32_t serial, uint32_t width, uint32_t height) {
    struct Pipeline *pipeline = data;
    zwlr_layer_surface_v1_ack_configure(pipeline->layer_surface, serial);

    if (pipeline->shm) {
        if (pipeline->shm->width == width && pipeline->shm->height == height)
            return;
        shm_destroy(pipeline->shm);
    }

    pipeline->shm = shm_create(width, height, WL_SHM_FORMAT_XRGB8888);
    pipeline_render(pipeline);
}

void pipeline_render(struct Pipeline *pipeline) {
    if (!pipeline || !pipeline->shm)
        return;

    int x = 0, y = 0;
    cairo_surface_t *image = cairo_image_surface_create_for_data(shm_data(pipeline->shm),
            CAIRO_FORMAT_ARGB32, pipeline->shm->width, pipeline->shm->height, pipeline->shm->stride);
    cairo_t *painter = cairo_create(image);
    pango_cairo_update_context(painter, pipeline->context);

    struct PipelineCallback *callback;
    for (int i = 0; i < pipeline->callbacks->length; i++) {
        pipeline->current = i;
        callback = pipeline->callbacks->data[i];
        callback->listener->render(pipeline, callback->data, painter, &x, &y);
    }

    wl_surface_attach(pipeline->surface, shm_buffer(pipeline->shm), 0, 0);
    wl_surface_damage(pipeline->surface, 0, 0, pipeline->shm->width, pipeline->shm->height);
    wl_surface_commit(pipeline->surface);

    cairo_destroy(painter);
    cairo_surface_destroy(image);

    shm_flip(pipeline->shm);
    pipeline->invalid = 0;
}

void pipeline_show(struct Pipeline *pipeline, struct wl_output *output) {
    if (!pipeline || !output || pipeline_is_visible(pipeline))
        return;

    pipeline->surface = wl_compositor_create_surface(compositor);
    pipeline->layer_surface = zwlr_layer_shell_v1_get_layer_surface(shell, pipeline->surface, output,
            ZWLR_LAYER_SHELL_V1_LAYER_BOTTOM, "doom.dwl-bar");
    zwlr_layer_surface_v1_add_listener(pipeline->layer_surface, &layer_surface_listener, pipeline);

    int anchor = bar_top ? ZWLR_LAYER_SURFACE_V1_ANCHOR_TOP : ZWLR_LAYER_SURFACE_V1_ANCHOR_BOTTOM;
    zwlr_layer_surface_v1_set_anchor(pipeline->layer_surface,
                                     anchor | ZWLR_LAYER_SURFACE_V1_ANCHOR_RIGHT | ZWLR_LAYER_SURFACE_V1_ANCHOR_LEFT);

    int height = pipeline->font->height + 2;
    zwlr_layer_surface_v1_set_size(pipeline->layer_surface, 0, height);
    zwlr_layer_surface_v1_set_exclusive_zone(pipeline->layer_surface, height);
    wl_surface_commit(pipeline->surface);
}

void pipeline_set_colorscheme(struct Pipeline* pipeline, const int **scheme) {
    for (int i = 0; i < 4; i++) {
        pipeline->foreground[i] = scheme[0][i];
        pipeline->background[i] = scheme[1][i];
    }
}

void pipeline_color_foreground(struct Pipeline* pipeline, cairo_t *painter) {
    set_color(painter, pipeline->foreground);
}

void pipeline_color_background(struct Pipeline* pipeline, cairo_t *painter) {
    set_color(painter, pipeline->background);
}

void set_color(cairo_t *painter, const int rgba[4]) {
    cairo_set_source_rgba(painter, rgba[0]/255.0, rgba[1]/255.0, rgba[2]/255.0, rgba[3]/255.0);
}
