#include <locus.h>
#include "wifi/wifi.h"
#include "battery/battery.h"
#include "clock/clock.h"
#include <glib.h>
#include <stdio.h>
#include <string.h>
#include "main.h"

Locus status_bar;

void draw_status_bar(cairo_t *cr, int width, int height){
    cairo_set_source_rgba(cr, 0, 0, 0, 0.7);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);
    draw_wifi(cr, status_bar.width * 3 / 100, status_bar.height * 85 / 100, status_bar.width * 1 / 100, status_bar.height * 10 / 100);
    draw_battery(cr, status_bar.width * 95 / 100, status_bar.height / 6, status_bar.width * 4 / 100, status_bar.height * 75 / 100);
    draw_clock(cr, status_bar.width * 46 / 100, status_bar.height / 20, status_bar.width * 7 / 100, status_bar.height * 85 / 100 );
}

void *glib(void *arg) {
    GMainContext *context = g_main_context_new();
    g_main_context_push_thread_default(context);
    GMainLoop *loop = g_main_loop_new(context, FALSE);
    init_battery();
    init_wifi();
    init_clock();
    g_main_loop_run(loop);
    g_main_loop_unref(loop);
    g_main_context_pop_thread_default(context);
    g_main_context_unref(context);
    return NULL;
}

int main(int argc, char *argv[]){
    locus_init(&status_bar, 100, 2);
    locus_create_layer_surface(&status_bar, "locus-shell", ZWLR_LAYER_SHELL_V1_LAYER_TOP, ZWLR_LAYER_SURFACE_V1_ANCHOR_TOP, 1);
    locus_set_draw_callback(&status_bar, draw_status_bar);
    GThread *glib_thread = g_thread_new("glib-main-loop", glib, NULL);
    locus_run(&status_bar);
    g_thread_join(glib_thread);
    return 0;
}
