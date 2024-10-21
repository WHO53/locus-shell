#ifndef BATTERY_H
#define BATTERY_H

#include <cairo.h>
#include <glib.h>

extern volatile gboolean redraw_battery_icon;

void draw_battery_icon(cairo_t *cr, int x, int y, int width, int height);
void init_battery();

#endif // BATTERY_H
