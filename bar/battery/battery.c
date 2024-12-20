#include <stdio.h>
#include <string.h>
#include <upower.h>
#include <locus.h>
#include <locus-ui.h>
#include <glib-object.h>
#include <glib.h>
#include "../bar.h"
#include "cairo.h"

static guint state;
static gdouble percentage;

static void on_battery_changed(UpDevice *device, GParamSpec *pspec) {
    g_object_get(device, 
            "percentage", &percentage,
            "state", &state,
            NULL);

    bar.redraw = 1;
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
        icon_name = "battery-missing-symbolic";
    } else if (state == 1 || state == 4) {
        if (percentage >= 90) {
            icon_name = "battery-level-100-charged-symbolic";
        } else if (percentage >= 80) {
            icon_name = "battery-level-90-charging-symbolic";
        } else if (percentage >= 70) {
            icon_name = "battery-level-80-charging-symbolic";
        } else if (percentage >= 60) {
            icon_name = "battery-level-70-charging-symbolic";
        } else if (percentage >= 50) {
            icon_name = "battery-level-60-charging-symbolic";
        } else if (percentage >= 40) {
            icon_name = "battery-level-50-charging-symbolic";
        } else if (percentage >= 30) {
            icon_name = "battery-level-40-charging-symbolic";
        } else if (percentage >= 20) {
            icon_name = "battery-level-30-charging-symbolic";
        } else if (percentage >= 10) {
            icon_name = "battery-level-20-charging-symbolic";
        } else if (percentage >= 0) {
            icon_name = "battery-level-10-charging-symbolic";
        }
    } else {
        if (percentage >= 90) {
            icon_name = "battery-level-100-symbolic";
        } else if (percentage >= 80) {
            icon_name = "battery-level-90-symbolic";
        } else if (percentage >= 70) {
            icon_name = "battery-level-80-symbolic";
        } else if (percentage >= 60) {
            icon_name = "battery-level-70-symbolic";
        } else if (percentage >= 50) {
            icon_name = "battery-level-60-symbolic";
        } else if (percentage >= 40) {
            icon_name = "battery-level-50-symbolic";
        } else if (percentage >= 30) {
            icon_name = "battery-level-40-symbolic";
        } else if (percentage >= 20) {
            icon_name = "battery-level-30-symbolic";
        } else if (percentage >= 10) {
            icon_name = "battery-level-20-symbolic";
        } else if (percentage >= 0) {
            icon_name = "battery-level-10-symbolic";
        }
    }

    locus_icon(cr, x, y, icon_name, width, height);

    char percentage_text[16];
    if (percentage == 100) {
        snprintf(percentage_text, sizeof(percentage_text), "%.0f", percentage);
    } else {
        snprintf(percentage_text, sizeof(percentage_text), "%.0f%%", percentage);
    }
    cairo_set_source_rgba(cr, 1, 1, 1, 1);
    locus_text(cr, percentage_text, x + ( width * 0.37) , y * 0.95, height * 0.85, BOLD);
}
