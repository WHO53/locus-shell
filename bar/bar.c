#include <locus.h>
#include "bar.h"
#include <glib.h>
#include <librsvg/rsvg.h>

Locus bar;

void draw_bar(cairo_t *cr, int width, int height){
    cairo_set_source_rgba(cr, 0.97, 0.33, 0.29, 0.9);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);
    draw_wifi(cr, width * 0.85 , height * 0.005, width / 10, height * 0.99);
    draw_battery(cr, width * 0.89, height * 0.115, width / 10 , height * 0.99);
    draw_clock(cr, width * 0.45, height * 0.005, width / 10, height * 0.99);
}

void *glib(void *arg) {
    GMainContext *context = g_main_context_new();
    g_main_context_push_thread_default(context);
    GMainLoop *loop = g_main_loop_new(context, FALSE);
    init_battery();
    init_clock();
    init_wifi();
    g_main_loop_run(loop);
    g_main_loop_unref(loop);
    g_main_context_pop_thread_default(context);
    g_main_context_unref(context);
    return NULL;
}

int icon_file_exists(const char *path) {
    FILE *file = fopen(path, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

char *find_bar_icon(const char *icon_name) {
    static char path[1024];
    const char *icon_dirs[] = {
        "/home/droidian/temp/Fluent-grey-dark/symbolic/status/",
        "/usr/share/icons/hicolor/scalable/apps/",
        "/usr/share/icons/hicolor/128x128/apps/",
        "/usr/share/pixmaps/",
        NULL
    };
    
    for (int i = 0; icon_dirs[i] != NULL; ++i) {
        snprintf(path, sizeof(path), "%s%s.png", icon_dirs[i], icon_name);
        if (icon_file_exists(path)) return path;
        snprintf(path, sizeof(path), "%s%s.svg", icon_dirs[i], icon_name);
        if (icon_file_exists(path)) return path;
        snprintf(path, sizeof(path), "%s%s-symbolic.svg", icon_dirs[i], icon_name);
        if (icon_file_exists(path)) return path;
    }

    return NULL;
}

void draw_bar_svg_icon(cairo_t *cr, int x, int y, const char *icon_name, int width, int height) {
    char *icon_path = find_bar_icon(icon_name);
    cairo_surface_t *icon_surface = NULL;

    if (icon_path != NULL) {
        if (strstr(icon_path, ".svg")) {
            RsvgHandle *svg = rsvg_handle_new_from_file(icon_path, NULL);
            RsvgRectangle viewport = { 0, 0, width, height };
            if (svg) {
                cairo_save(cr);
                cairo_translate(cr, x, y);
                rsvg_handle_render_document(svg, cr, &viewport, NULL);
                cairo_restore(cr);
                g_object_unref(svg);
            }
        } else {
            icon_surface = cairo_image_surface_create_from_png(icon_path);
            if (cairo_surface_status(icon_surface) == CAIRO_STATUS_SUCCESS) {
                double icon_width = cairo_image_surface_get_width(icon_surface);
                double icon_height = cairo_image_surface_get_height(icon_surface);
                cairo_save(cr);
                cairo_translate(cr, x, y);
                cairo_scale(cr, (double)width / icon_width, (double)height / icon_height);
                cairo_set_source_surface(cr, icon_surface, 0, 0);
                cairo_paint(cr);
                cairo_restore(cr);
                cairo_surface_destroy(icon_surface);
            }
        }
    }

    if (!icon_surface && !icon_path) {
        cairo_set_source_rgb(cr, 0.2, 0.6, 0.8);
        cairo_rectangle(cr, x, y, width, height);
        cairo_fill(cr);
    }
}
