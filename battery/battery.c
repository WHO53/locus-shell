#include <stdio.h>
#include <string.h>
#include <upower.h>
#include "battery.h"
#include "cairo.h"
#include "glib-object.h"
#include "glib.h"

guint state;
gdouble percentage;

static void on_battery_changed(UpDevice *device, GParamSpec *pspec) {
    g_object_get(device, 
            "percentage", &percentage,
            "state", &state,
            NULL);

    fprintf(stderr, "%.2f%%, state: %d\n", percentage, state);
}

void battery() {
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
}
