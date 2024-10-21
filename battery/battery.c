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

    fprintf(stderr, "%.2f%%, state: %d\n", percentage, state);
    locus_set_partial_draw_callback(&app, draw_battery_icon);
    locus_req_partial_redraw(&app, (app.width * 5) / 100, (app.height * 8) / 100, 100, 50);
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

void draw_battery_icon(cairo_t *cr, int x, int y, int width, int height) {
    cairo_set_line_width(cr, 2);
    if (state == 2 ) {
        cairo_set_source_rgba(cr, 1, 1, 1, 1);
    } else {
        cairo_set_source_rgba(cr, 1, 1, 0, 1);
    }
    cairo_rectangle(cr, x, y, width, height);
    cairo_stroke(cr);
    fprintf(stderr, "%.2f%%\n", percentage);
}
