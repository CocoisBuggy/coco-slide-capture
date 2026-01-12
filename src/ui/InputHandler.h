#pragma once

#include <gtk/gtk.h>

class MainWindow;

class InputHandler {
 public:
  static gboolean on_key_press(GtkEventControllerKey *controller, guint keyval,
                               guint keycode, GdkModifierType state,
                               gpointer user_data);

  static void update_star_display(GtkWidget *label, int rating);
};