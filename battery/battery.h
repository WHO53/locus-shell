#ifndef BATTERY_H
#define BATTERY_H

#include <cairo.h>

void init_battery();
void draw_battery_icon(cairo_t *cr, int x, int y, int width, int height);

#endif // BATTERY_H
