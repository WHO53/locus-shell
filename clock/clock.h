#ifndef CLOCK_H
#define CLOCK_H

#include <cairo.h>

void init_clock();
void draw_clock(cairo_t *cr, int x, int y, int width, int height);

#endif // CLOCK_H
