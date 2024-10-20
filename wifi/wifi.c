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

    // Create a new NMClient object
    client = nm_client_new(NULL, NULL);
    if (!client) {
        fprintf(stderr, "Error: Could not create NMClient.\n");
        return -1;  // Indicate an error with -1
    }

    // Get the primary connection
    active_conn = nm_client_get_primary_connection(client);
    if (!active_conn) {
        fprintf(stderr, "Error: No active connection.\n");
        g_object_unref(client);
        return -1;
    }

    // Check if it's a Wi-Fi connection
    if (strcmp(nm_active_connection_get_connection_type(active_conn), NM_SETTING_WIRELESS_SETTING_NAME) != 0) {
        fprintf(stderr, "Error: Active connection is not Wi-Fi.\n");
        g_object_unref(client);
        return -1;
    }

    // Get the devices associated with the active connection
    devices = nm_active_connection_get_devices(active_conn);
    if (!devices || devices->len == 0) {
        fprintf(stderr, "Error: No devices associated with the active connection.\n");
        g_object_unref(client);
        return -1;
    }

    // Get the first device (assuming it's the Wi-Fi device)
    wifi_device = NM_DEVICE_WIFI(g_ptr_array_index(devices, 0));
    if (!wifi_device) {
        fprintf(stderr, "Error: Could not get Wi-Fi device.\n");
        g_object_unref(client);
        return -1;
    }

    // Get the current access point
    ap = nm_device_wifi_get_active_access_point(wifi_device);
    if (!ap) {
        fprintf(stderr, "Error: No active access point.\n");
        g_object_unref(client);
        return -1;
    }

    // Get the signal strength
    strength = nm_access_point_get_strength(ap);

    // Clean up
    g_object_unref(client);

    // Map the signal strength to the range 0 to 4
    if (strength <= 20) return 0;
    if (strength <= 40) return 1;
    if (strength <= 60) return 2;
    if (strength <= 80) return 3;
    return 4; // For strengths greater than 80%
}

void draw_wifi_icon(cairo_t *cr, int x, int y, int width, int height) {
    cairo_set_line_width(cr, 2.0);
    int str = get_wifi_str();
    for (int s = 0; s < 4; s++) {
        if ( s < str ) {
            cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 1);
        } else {
            cairo_set_source_rgba(cr, 0.5, 0.5, 0.5, 1);
        }
            cairo_arc(cr, x, y, 3.5 * (s + 1), M_PI, 2 * M_PI);
            cairo_stroke(cr);
    }
}
