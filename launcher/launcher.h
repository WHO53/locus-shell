#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <locus.h>

extern Locus launcher;

void draw_launcher(cairo_t *cr, int width, int height);

int compare_apps(const void *a, const void *b);

void process_desktop_directory();

void touch_launcher(int32_t id, double x, double y, int state);

void launcher_init();

void launcher_free_apps();

#endif // LAUNCHER_H

