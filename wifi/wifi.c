#include <math.h>
#include <stdio.h>
#include <NetworkManager.h>
#include <nm-utils.h>
#include <cairo.h>
#include "wifi.h"

static int get_wifi_str() {
    NMClient *client;
    NMActiveConnection *active_conn;
    NMAccessPoint *ap;
    NMDeviceWifi *wifi_device;
    guint8 strength;
    const GPtrArray *devices;

    client = nm_client_new(NULL, NULL);
    if (!client) {
        return -1;
    }

    active_conn = nm_client_get_primary_connection(client);
    if (!active_conn) {
        g_object_unref(client);
        return -1;
    }

    if (strcmp(nm_active_connection_get_connection_type(active_conn), NM_SETTING_WIRELESS_SETTING_NAME) != 0) {
        g_object_unref(client);
        return -1;
    }

    devices = nm_active_connection_get_devices(active_conn);
    if (!devices || devices->len == 0) {
        g_object_unref(client);
        return -1;
    }

    wifi_device = NM_DEVICE_WIFI(g_ptr_array_index(devices, 0));
    if (!wifi_device) {
        g_object_unref(client);
        return -1;
    }

    ap = nm_device_wifi_get_active_access_point(wifi_device);
    if (!ap) {
        g_object_unref(client);
        return -1;
    }

    strength = nm_access_point_get_strength(ap);

    g_object_unref(client);

    if (strength <= 20) return 0;
    if (strength <= 40) return 1;
    if (strength <= 60) return 2;
    if (strength <= 80) return 3;
    return 4;
}

void draw_wifi_icon(cairo_t *cr, int x, int y, int width, int height) {
    cairo_set_line_width(cr, 2.0);
    int str = get_wifi_str();
    for (int s = 0; s < 4; s++) {
        if (str == -1 ) {
            cairo_set_source_rgba(cr, 1.0, 0, 0, 1);
        } else
        if ( s < str ) {
            cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 1);
        } else {
            cairo_set_source_rgba(cr, 0.5, 0.5, 0.5, 1);
        }
            cairo_arc(cr, x, y, 3.5 * (s + 1), M_PI, 2 * M_PI);
            cairo_stroke(cr);
    }
}
