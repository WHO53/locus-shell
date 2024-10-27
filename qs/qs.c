#include <locus.h>
#include <locus-ui.h>
#include "qs.h"
#include "cairo.h"
#include <glib.h>
#include <librsvg/rsvg.h>

Locus qs;
void qs_widget(cairo_t *cr, double center_x, double center_y, double width, double height, 
        const char *icon_name, const char *text, double icon_size) {
    cairo_set_source_rgba(cr, 0.99, 0.33, 0.29, 0.8);
    locus_rectangle(cr, center_x, center_y, width, height, 25,
            TOP_LEFT | TOP_RIGHT | BOTTOM_LEFT | BOTTOM_RIGHT);
    locus_icon(cr, center_x * 0.30 , center_y, icon_name, icon_size, icon_size);
    cairo_set_source_rgba(cr, 1, 1, 1, 1);
    locus_text(cr, text, center_x, center_y, 0, height * 0.50, NORMAL);
}


void draw_dots(cairo_t *cr, double center_x, double center_y, int dot_count, double spacing, 
        double dot_size, int horizontal) {
    cairo_set_source_rgba(cr, 1, 1, 1, 1); 

    for (int i = 0; i < dot_count; i++) {
        double x, y;
        if (horizontal) {
            x = center_x + (i - (dot_count - 1) / 2.0) * spacing; 
            y = center_y; 
        } else {
            x = center_x; 
            y = center_y + (i - (dot_count - 1) / 2.0) * spacing; 
        }

        cairo_arc(cr, x, y, dot_size, 0, 2 * M_PI);
        cairo_fill(cr); 
    }
}


void draw_qs(cairo_t *cr, int width, int height){
    cairo_set_source_rgba(cr, 0, 0, 0, 0.8);
    locus_rectangle(cr, width * 0.50, height * 0.50, width, height, 25,
            TOP_LEFT | TOP_RIGHT | BOTTOM_LEFT | BOTTOM_RIGHT);
    qs_widget(cr, width * 0.50, height * 0.11, width * 0.85, height * 0.16, "network-wireless-signal-good", "IDK", height * 0.12);
    qs_widget(cr, width * 0.50, height * 0.30, width * 0.85, height * 0.16, "blueberry-tray-active", "IDK", height * 0.12);
    locus_slider(cr, width * 0.50, height * 0.49, width * 0.85, height * 0.16, 50);
    locus_icon(cr, width * 0.50 * 0.30, height * 0.49, "display-brightness", height * 0.12, height * 0.12);
    locus_slider(cr, width * 0.50, height * 0.68, width * 0.85, height * 0.16, 50);
    locus_icon(cr, width * 0.50 * 0.30, height * 0.68, "audio-volume-high", height * 0.12, height * 0.12);
    qs_widget(cr, width * 0.50, height * 0.87, width * 0.85, height * 0.16, "preferences-desktop-multitasking", "Mivi Duopods", height * 0.12);
}
