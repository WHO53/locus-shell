#include <stdio.h>
#include <string.h>
#include <time.h>
#include <locus.h>
#include <locus-ui.h>
#include <glib.h>
#include "../bar.h"

static int current_hour = -1;
static int current_minute = -1;
static guint timer_id = 0;

static gboolean update_time(gpointer user_data) {
    time_t now = time(NULL);
    struct tm *time_info = localtime(&now);
    
    current_hour = time_info->tm_hour;
    current_minute = time_info->tm_min;
    
    bar.redraw = 1;
    return G_SOURCE_CONTINUE;
}

void init_clock() {
    update_time(NULL);
    
    GMainContext *context = g_main_context_get_thread_default();
    if (!context) {
        context = g_main_context_default();
    }
    
    GSource *source = g_timeout_source_new_seconds(60);
    g_source_set_callback(source, update_time, NULL, NULL);
    timer_id = g_source_attach(source, context);
    g_source_unref(source);
    
}

void draw_clock(cairo_t *cr, int x, int y, int width, int height) {
    char hour_text[3];  
    char minute_text[3]; 

    snprintf(hour_text, sizeof(hour_text), "%02d", current_hour);
    snprintf(minute_text, sizeof(minute_text), "%02d", current_minute);

    cairo_set_source_rgba(cr, 1, 1, 1, 1); 

    locus_text(cr, hour_text, x, y, width, width * 0.70, NORMAL);  
    
    locus_text(cr, minute_text, x, y + (width * 0.60), width, width * 0.70, NORMAL);  
}
