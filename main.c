#include <locus.h>
#include "bar/bar.h"
#include "launcher/launcher.h"
#include <glib.h>
#include <stdio.h>

Locus *instance[] = {&bar, &launcher};

int main(int argc, char *argv[]){
    locus_init(&bar, 100, 2);
    locus_create_layer_surface(&bar, "locus-sidebar", ZWLR_LAYER_SHELL_V1_LAYER_TOP, ZWLR_LAYER_SURFACE_V1_ANCHOR_TOP, 1);
    locus_set_draw_callback(&bar, draw_bar);
    GThread *glib_thread = g_thread_new("glib-main-loop", glib, NULL);


    locus_init(&launcher, 100, 98);
    locus_create_layer_surface(&launcher, "locus-launcher", ZWLR_LAYER_SHELL_V1_LAYER_BOTTOM, ZWLR_LAYER_SURFACE_V1_ANCHOR_BOTTOM, 0);
    locus_set_draw_callback(&launcher, draw_launcher);
    locus_set_touch_callback(&launcher, touch_launcher);
    launcher_init();

    locus_run_multi(instance, 2);
    g_thread_join(glib_thread);
    launcher_free_apps();
    locus_cleanup(&bar);
    locus_cleanup(&launcher);
    return 0;
}
