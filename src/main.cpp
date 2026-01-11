#include <gtk-4.0/gtk/gtk.h>

#include "ui/MainWindow.h"

static void activate(GtkApplication *app, gpointer user_data) {
  MainWindow *window = new MainWindow(app);
  window->show();
}

int main(int argc, char **argv) {
  GtkApplication *app =
      gtk_application_new("com.example.film", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  int status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
