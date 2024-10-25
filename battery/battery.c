#include <stdio.h>
#include <string.h>
#include <upower.h>
#include "battery.h"
#include "cairo.h"
#include "glib-object.h"
#include "glib.h"
#include "../main.h"
#include "../svg.h"
#include "locus.h"

static guint state;
static gdouble percentage;

static void on_battery_changed(UpDevice *device, GParamSpec *pspec) {
    g_object_get(device, 
            "percentage", &percentage,
            "state", &state,
            NULL);

    locus_set_partial_draw_callback(&app, draw_battery);
    locus_req_partial_redraw(&app, app.width * 0.90, app.height * 0.005, app.width / 10, app.height * 0.99);
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
    const char *icon_name;

    if (percentage < 0 || percentage > 100) {
        icon_name = "battery-missing";
    } else if (state == 1 || state == 4) {
        if (percentage >= 90) {
            icon_name = "battery-level-100-charged";
        } else if (percentage >= 80) {
            icon_name = "battery-level-90-charging";
        } else if (percentage >= 70) {
            icon_name = "battery-level-80-charging";
        } else if (percentage >= 60) {
            icon_name = "battery-level-70-charging";
        } else if (percentage >= 50) {
            icon_name = "battery-level-60-charging";
        } else if (percentage >= 40) {
            icon_name = "battery-level-50-charging";
        } else if (percentage >= 30) {
            icon_name = "battery-level-40-charging";
        } else if (percentage >= 20) {
            icon_name = "battery-level-30-charging";
        } else if (percentage >= 10) {
            icon_name = "battery-level-20-charging";
        } else if (percentage >= 0) {
            icon_name = "battery-level-10-charging";
        }
    } else {
        if (percentage >= 90) {
            icon_name = "battery-level-100";
        } else if (percentage >= 80) {
            icon_name = "battery-level-90";
        } else if (percentage >= 70) {
            icon_name = "battery-level-80";
        } else if (percentage >= 60) {
            icon_name = "battery-level-70";
        } else if (percentage >= 50) {
            icon_name = "battery-level-60";
        } else if (percentage >= 40) {
            icon_name = "battery-level-50";
        } else if (percentage >= 30) {
            icon_name = "battery-level-40";
        } else if (percentage >= 20) {
            icon_name = "battery-level-30";
        } else if (percentage >= 10) {
            icon_name = "battery-level-20";
        } else if (percentage >= 0) {
            icon_name = "battery-level-10";
        }
    }

    draw_svg_icon(cr, x, y, icon_name, width, height);
}
