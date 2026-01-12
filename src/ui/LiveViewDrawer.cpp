#include "LiveViewDrawer.h"

#include <cairo.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include <algorithm>

#include "ui/MainWindow.h"

void LiveViewDrawer::draw_live_view(GtkDrawingArea *area, cairo_t *cr,
                                    int width, int height, gpointer data) {
  MainWindow *self = static_cast<MainWindow *>(data);
  if (self->renderer && self->renderer->getCurrentImage()) {
    GdkPixbuf *pixbuf = self->renderer->getCurrentImage();
    int img_width = gdk_pixbuf_get_width(pixbuf);
    int img_height = gdk_pixbuf_get_height(pixbuf);

    // Calculate scale to fit while maintaining aspect ratio
    double scale_x = (double)width / img_width;
    double scale_y = (double)height / img_height;
    double scale = std::min(scale_x, scale_y);

    int new_width = img_width * scale;
    int new_height = img_height * scale;

    GdkPixbuf *scaled = gdk_pixbuf_scale_simple(pixbuf, new_width, new_height,
                                                GDK_INTERP_BILINEAR);
    if (scaled) {
      // Center the image
      int x = (width - new_width) / 2;
      int y = (height - new_height) / 2;
      gdk_cairo_set_source_pixbuf(cr, scaled, x, y);
      cairo_paint(cr);
      g_object_unref(scaled);
    }
  } else {
    // Draw a black rectangle as placeholder
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);

    // Draw some text
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 24);
    cairo_move_to(cr, width / 2 - 50, height / 2);
    cairo_show_text(cr, "Live View");
  }
}