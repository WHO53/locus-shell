#include <locus.h>
#include "wifi/wifi.h"

void draw_status_bar(cairo_t *cr, int width, int height){
    cairo_set_source_rgba(cr, 0.1, 0.1, 0.1, 1);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);
    draw_wifi_icon(cr, (width * 3) / 100, (height * 80) / 100, 10, 10);
}

int main(){
    Locus app;
    locus_init(&app, 100, 2);
    locus_create_layer_surface(&app, ZWLR_LAYER_SHELL_V1_LAYER_TOP, ZWLR_LAYER_SURFACE_V1_ANCHOR_TOP);
    locus_set_draw_callback(&app, draw_status_bar);
    locus_run(&app);
}
