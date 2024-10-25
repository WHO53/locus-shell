#include <stdio.h>
#include <string.h>
#include <upower.h>
#include "battery.h"
#include "cairo.h"
#include "glib-object.h"
#include "glib.h"
#include "../main.h"
#include "locus.h"

static guint state;
static gdouble percentage;

static void on_battery_changed(UpDevice *device, GParamSpec *pspec) {
    g_object_get(device, 
            "percentage", &percentage,
            "state", &state,
            NULL);

    locus_set_partial_draw_callback(&status_bar, draw_battery);
    locus_req_partial_redraw(&status_bar, status_bar.width * 95 / 100, status_bar.height / 6, status_bar.width * 4 / 100, status_bar.height * 75 / 100);
}

void init_battery() {
    UpClient *client = NULL;
    UpDevice *device = NULL;

    client = up_client_new();
    if (!client) {
    }

    device = up_client_get_display_device(client);
    if (!device) {
    }

    g_signal_connect(device, "notify::percentage", G_CALLBACK(on_battery_changed), NULL);
    g_signal_connect(device, "notify::state", G_CALLBACK(on_battery_changed), NULL);

    on_battery_changed(device, NULL);
}

void draw_battery(cairo_t *cr, int x, int y, int width, int height) {
    cairo_set_source_rgba(cr, 0, 0, 0, 1);
    cairo_rectangle(cr, x, y, width, height);
    cairo_fill(cr);

    cairo_set_line_width(cr, 2);

    if (percentage < 20) {
        cairo_set_source_rgba(cr, 1, 0, 0, 1);
    } 
    if (state == 1 || state == 4) {
        cairo_set_source_rgba(cr, 0, 1, 0, 1);
    } else if (state == 2){
        cairo_set_source_rgba(cr, 0.5, 0.5, 0.5, 1);
    } else {
        cairo_set_source_rgba(cr, 1, 1, 0, 1);
    }

    cairo_rectangle(cr, x, y, width * percentage / 100, height);
    cairo_fill(cr);
    cairo_stroke(cr);

    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, height * 0.9);

    char percentage_text[3];
    snprintf(percentage_text, sizeof(percentage_text), "%.2f%%", percentage);

    cairo_text_extents_t extents;
    cairo_text_extents(cr, percentage_text, &extents);
    double text_x = x + (width - extents.width) / 2;
    double text_y = y + (height + extents.height) / 2;
    cairo_set_source_rgba(cr, 1, 1, 1, 1);
    cairo_move_to(cr, text_x, text_y);
    cairo_show_text(cr, percentage_text);
}
