#include <locus.h>
#include "bar/bar.h"
#include <glib.h>

int main(int argc, char *argv[]){
    locus_init(&bar, 100, 2);
    locus_create_layer_surface(&bar, "locus-sidebar", ZWLR_LAYER_SHELL_V1_LAYER_TOP, ZWLR_LAYER_SURFACE_V1_ANCHOR_TOP, 1);
    locus_set_draw_callback(&bar, draw_bar);
    GThread *glib_thread = g_thread_new("glib-main-loop", glib, NULL);
    locus_run(&bar);
    g_thread_join(glib_thread);
    return 0;
}
