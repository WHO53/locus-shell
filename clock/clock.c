#include <stdio.h>
#include <string.h>
#include <time.h>
#include "clock.h"
#include <cairo.h>
#include <glib.h>
#include "../main.h"
#include <locus.h>

static int current_hour = -1;
static int current_minute = -1;
static guint timer_id = 0;

static gboolean update_time(gpointer user_data) {
    time_t now = time(NULL);
    struct tm *time_info = localtime(&now);
    
    current_hour = time_info->tm_hour;
    current_minute = time_info->tm_min;
    
    locus_set_partial_draw_callback(&status_bar, draw_clock);
    locus_req_partial_redraw(&status_bar, status_bar.width * 46 / 100, status_bar.height / 20, status_bar.width * 7 / 100, status_bar.height * 85 / 100 );
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
    cairo_set_source_rgba(cr, 0, 0, 0, 1);
    cairo_rectangle(cr, x, y, width, height);
    cairo_fill(cr);
    
    char time_text[6];
    snprintf(time_text, sizeof(time_text), "%02d:%02d", 
            current_hour, current_minute);
    
    cairo_select_font_face(cr, "Monofur Nerd Font", 
            CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, height * 0.9);
    
    cairo_text_extents_t extents;
    cairo_text_extents(cr, time_text, &extents);
    
    cairo_set_source_rgba(cr, 1, 1, 1, 1);
    cairo_move_to(cr, 
            x + (width - extents.width) / 2,
            y + (height - extents.height) / 2 + extents.height);
    cairo_show_text(cr, time_text);
}
