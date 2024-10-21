#include <locus.h>
#include "wifi/wifi.h"
#include "battery/battery.h"
#include <glib.h>
#include <stdio.h>
#include <string.h>
#include "main.h"

Locus app;

void draw_status_bar(cairo_t *cr, int width, int height){
    cairo_set_source_rgba(cr, 0.1, 0.1, 0.1, 1);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);
    draw_wifi_icon(cr, (width * 3) / 100, (height * 80) / 100, 10, 10);
    draw_battery_icon(cr, (width * 5) / 100, (height * 8) / 100, 100, 50);
}

void *glib(void *arg) {
    GMainContext *context = g_main_context_new();
    g_main_context_push_thread_default(context);
    GMainLoop *loop = g_main_loop_new(context, FALSE);
    init_battery();
    g_main_loop_run(loop);
    g_main_loop_unref(loop);
    g_main_context_pop_thread_default(context);
    g_main_context_unref(context);
    return NULL;
}

int main(int argc, char *argv[]){
    locus_init(&app, 100, 2);
    locus_create_layer_surface(&app, ZWLR_LAYER_SHELL_V1_LAYER_TOP, ZWLR_LAYER_SURFACE_V1_ANCHOR_TOP);
    locus_set_draw_callback(&app, draw_status_bar);
    GThread *glib_thread = g_thread_new("glib-main-loop", glib, NULL);
    locus_run(&app);
    g_thread_join(glib_thread);
    return 0;
}
