#ifndef BAR_H
#define BAR_H

#include <locus.h>

extern Locus bar;

void draw_bar(cairo_t *cr, int width, int height);
void *glib(void *arg);

void draw_bar_svg_icon(cairo_t *cr, int x, int y, const char *icon_name, int width, int height);

void init_battery();
void draw_battery(cairo_t *cr, int x, int y, int width, int height);

void init_clock();
void draw_clock(cairo_t *cr, int x, int y, int width, int height);

void init_wifi();
void draw_wifi(cairo_t *cr, int x, int y, int width, int height);

#endif // BAR_H

