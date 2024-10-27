#include <locus.h>
#include <locus-ui.h>
#include "bar.h"
#include <glib.h>
#include <librsvg/rsvg.h>

Locus bar;

void draw_bar(cairo_t *cr, int width, int height){
    cairo_set_source_rgba(cr, 0.97, 0.33, 0.29, 0.9);
    locus_rectangle(cr, width * 0.50, height * 0.50, width, height, 25, 0);
    draw_wifi(cr, width * 0.90 , height * 0.50, width * .10, height * 0.99);
    draw_battery(cr, width * 0.94, height * 0.60, width * 0.10 , height * 0.99);
    draw_clock(cr, width * 0.50, height * 0.50, width * .10, height * 0.99);
}

void *glib(void *arg) {
    GMainContext *context = g_main_context_new();
    g_main_context_push_thread_default(context);
    GMainLoop *loop = g_main_loop_new(context, FALSE);
    init_battery();
    init_clock();
    init_wifi();
    g_main_loop_run(loop);
    g_main_loop_unref(loop);
    g_main_context_pop_thread_default(context);
    g_main_context_unref(context);
    return NULL;
}
