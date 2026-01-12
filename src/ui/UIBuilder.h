#pragma once

#include <gtk/gtk.h>

class MainWindow;

class UIBuilder {
 public:
  static GtkWidget *create_main_window(GtkApplication *app,
                                       MainWindow *mainWindow);
  static GtkWidget *create_status_label();
  static GtkWidget *create_live_view_area();
  static GtkWidget *create_controls(MainWindow *mainWindow);
  static GtkWidget *create_star_rating_widget(MainWindow *mainWindow);
  static void setup_signals(GtkWidget *window, GtkWidget *batch_button,
                            GtkWidget *set_date_button, GtkWidget *calendar,
                            MainWindow *mainWindow);
};