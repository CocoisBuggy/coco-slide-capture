#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtk-4.0/gtk/gtk.h>
#include <string>
#include "camera/CameraManager.h"
#include "camera/LiveViewRenderer.h"

class MainWindow {
public:
    MainWindow(GtkApplication* app);
    ~MainWindow();

    void show();

private:
    // GTK widgets
    GtkWidget* window;
    GtkWidget* status_label;
    GtkWidget* live_view;
    GtkWidget* dir_entry;
    GtkWidget* date_display_label;
    GtkWidget* set_date_button;
    GtkWidget* popover;
    GtkWidget* calendar;

    // State
    std::string active_directory;
    std::string context_date;

    // Camera
    CameraManager* camMgr;
    LiveViewRenderer* renderer;

    // Callbacks
    static void draw_live_view(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer data);
    static gboolean on_key_press(GtkEventControllerKey *controller, guint keyval, guint keycode, GdkModifierType state, gpointer user_data);
    static void on_directory_changed(GtkEditable *editable, gpointer user_data);
    static void on_set_date_clicked(GtkButton *button, gpointer user_data);
    static void on_date_selected(GtkCalendar *calendar, gpointer user_data);
};

#endif // MAINWINDOW_H