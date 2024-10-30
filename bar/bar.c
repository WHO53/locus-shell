#include <locus.h>
#include <locus-ui.h>
#include "bar.h"
#include "cairo.h"
#include <glib.h>
#include <librsvg/rsvg.h>

Locus bar;

void draw_bar(cairo_t *cr, int width, int height){
    if (bar.state == 1) {
        locus_color(cr, 255, 255, 255, 1);
        locus_rectangle(cr, width * 0.10, height * 0.50, width * 0.20, height * 0.10, width * 0.20, TOP_RIGHT | BOTTOM_RIGHT);
    } else {
        locus_color(cr, 51, 194, 117, .7);
        locus_rectangle(cr, width * 0.50, height * 0.50, width, height, 0, 0);
        draw_wifi(cr, width * 0.50 , height * 0.025, width * .90, height * 0.99);
        draw_battery(cr, width * 0.50, height * 0.95, width * 0.90 , height * 0.99);
        draw_clock(cr, width * 0.50, height * 0.50, width * .99, height * 0.99);
    }
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
