#ifndef BATTERY_H
#define BATTERY_H

#include <cairo.h>

void draw_battery_icon(cairo_t *cr, int x, int y, int width, int height);
void battery();

#endif // BATTERY_H
