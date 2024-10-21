#include <math.h>
#include <stdio.h>
#include <NetworkManager.h>
#include <nm-utils.h>
#include <cairo.h>
#include <string.h>
#include "wifi.h"
#include "../main.h"
#include "glib-object.h"
#include "glib.h"
#include "glibconfig.h"

static NMClient *g_client = NULL;
static NMDeviceWifi *g_wifi_dev = NULL;

static void on_wifi_properties_changed(NMDeviceWifi *wifi_dev, GParamSpec *pspec, gpointer user_data) {
    locus_set_partial_draw_callback(&app, draw_wifi_icon);
    locus_req_partial_redraw(&app, app.width * 3 / 100, app.height * 75 / 100, 
                            app.width * 1 / 100, app.height * 10 / 100);
}

static void on_wireless_enabled_changed(NMClient *client, GParamSpec *pspec, gpointer user_data) {
    locus_set_partial_draw_callback(&app, draw_wifi_icon);
    locus_req_partial_redraw(&app, app.width * 3 / 100, app.height * 75 / 100, 
                            app.width * 1 / 100, app.height * 10 / 100);
}

void init_wifi() {
    GError *error = NULL;
    
    g_client = nm_client_new(NULL, &error);
    if (!g_client) {
        if (error) g_error_free(error);
        return;
    }
    
    const GPtrArray *devices = nm_client_get_devices(g_client);
    for (guint i = 0; i < devices->len; i++) {
        NMDevice *device = NM_DEVICE(g_ptr_array_index(devices, i));
        if (NM_IS_DEVICE_WIFI(device)) {
            g_wifi_dev = NM_DEVICE_WIFI(device);
            break;
        }
    }
    
    if (!g_wifi_dev) {
        g_object_unref(g_client);
        g_client = NULL;
        return;
    }
    
    g_signal_connect(g_wifi_dev, "notify::active-access-point",
                    G_CALLBACK(on_wifi_properties_changed), NULL);
    g_signal_connect(g_wifi_dev, "notify::strength",
                    G_CALLBACK(on_wifi_properties_changed), NULL);
    g_signal_connect(g_client, "notify::wireless-enabled",
                    G_CALLBACK(on_wireless_enabled_changed), NULL);
    
    on_wifi_properties_changed(g_wifi_dev, NULL, NULL);
}

static int get_wifi_str() {
    if (!g_client || !g_wifi_dev) {
        return -1;
    }
    
    NMAccessPoint *ap = nm_device_wifi_get_active_access_point(g_wifi_dev);
    if (!ap) {
        return -1;
    }
    
    guint8 strength = nm_access_point_get_strength(ap);
    
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
        if (str == -1) {
            cairo_set_source_rgba(cr, 1.0, 0, 0, 1);
        } else if (s < str) {
            cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 1);
        } else {
            cairo_set_source_rgba(cr, 0.5, 0.5, 0.5, 1);
        }
        cairo_arc(cr, x, y, 3.5 * (s + 1), M_PI, 2 * M_PI);
        cairo_stroke(cr);
    }
}
