#include <locus.h>
#include "main.h"
#include "battery/battery.h"
#include "clock/clock.h"
#include "wifi/wifi.h"
#include <glib.h>

Locus app;

void draw_bar(cairo_t *cr, int width, int height){
    cairo_set_source_rgba(cr, 0.2, 0.9, 0.5, 0.5);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);
    draw_wifi(cr, width * 0.87 , height * 0.005, width / 10, height * 0.99);
    draw_battery(cr, width * 0.92, height * 0.005, width / 10 , height * 0.99);
    draw_clock(cr, width * 0.45, height * 0.005, width / 10, height * 0.99);
}

void touch(int32_t id, double x, double y, int state) {
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

int main(int argc, char *argv[]){
    locus_init(&app, 100, 2);
    locus_create_layer_surface(&app, "locus-sidebar", ZWLR_LAYER_SHELL_V1_LAYER_TOP, ZWLR_LAYER_SURFACE_V1_ANCHOR_TOP, 1);
    locus_set_draw_callback(&app, draw_bar);
    locus_set_touch_callback(&app, touch);
    GThread *glib_thread = g_thread_new("glib-main-loop", glib, NULL);
    locus_run(&app);
    g_thread_join(glib_thread);
    return 0;
}
