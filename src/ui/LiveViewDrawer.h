#pragma once

#include <gtk/gtk.h>

class MainWindow;

class LiveViewDrawer {
 public:
  static void draw_live_view(GtkDrawingArea *area, cairo_t *cr, int width,
                             int height, gpointer data);
};