#include <gtk-4.0/gtk/gtk.h>
#include <iostream>
#include <string>
#include <cairo.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "CameraManager.h"
#include "LiveViewRenderer.h"

// Global variables for state
std::string active_directory = "";
std::string context_date = "";

// Camera and renderer
CameraManager* camMgr = nullptr;
LiveViewRenderer* renderer = nullptr;

// Callback for drawing the live view
static void draw_live_view(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer data) {
    LiveViewRenderer* rend = static_cast<LiveViewRenderer*>(data);
    if (rend && rend->getCurrentImage()) {
        gdk_cairo_set_source_pixbuf(cr, rend->getCurrentImage(), 0, 0);
        cairo_paint(cr);
    } else {
        // Draw a black rectangle as placeholder
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_rectangle(cr, 0, 0, width, height);
        cairo_fill(cr);

        // Draw some text
        cairo_set_source_rgb(cr, 1, 1, 1);
        cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, 24);
        cairo_move_to(cr, width/2 - 50, height/2);
        cairo_show_text(cr, "Live View");
    }
}

// Callback for key press
static gboolean on_key_press(GtkEventControllerKey *controller, guint keyval, guint keycode, GdkModifierType state, gpointer user_data) {
    if (keyval == GDK_KEY_space) {
        std::cout << "Spacebar pressed: Trigger capture" << std::endl;
        // In full app, trigger capture
        return TRUE;
    }
    return FALSE;
}

// Callback for directory entry changed
static void on_directory_changed(GtkEditable *editable, gpointer user_data) {
    const char *text = gtk_editable_get_text(editable);
    active_directory = text;
    std::cout << "Active directory set to: " << active_directory << std::endl;
}

// Callback for set date button
static void on_set_date_clicked(GtkButton *button, gpointer user_data) {
    GtkPopover *popover = GTK_POPOVER(user_data);
    gtk_popover_popup(popover);
}

// Callback for date selected
static void on_date_selected(GtkCalendar *calendar, gpointer user_data) {
    GtkLabel *label = GTK_LABEL(user_data);
    GDateTime *date = gtk_calendar_get_date(calendar);
    if (date) {
        char *str = g_date_time_format(date, "%Y-%m-%d");
        context_date = str;
        char *label_text = g_strdup_printf("Context Date: <b>%s</b>", str);
        gtk_label_set_markup(label, label_text);
        std::cout << "Context date set to: " << context_date << std::endl;
        g_free(str);
        g_free(label_text);
        g_date_time_unref(date);
    }
    // Hide popover
    GtkWidget *popover = gtk_widget_get_ancestor(GTK_WIDGET(calendar), GTK_TYPE_POPOVER);
    if (popover) gtk_popover_popdown(GTK_POPOVER(popover));
}

static void activate(GtkApplication *app, gpointer user_data) {
    // Create window
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Canon Camera Control");
    gtk_window_set_default_size(GTK_WINDOW(window), 1200, 800);

    // Create main vertical box
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_window_set_child(GTK_WINDOW(window), main_box);

    // Status label at top
    GtkWidget *status_label = gtk_label_new("Status: Initializing");
    gtk_label_set_use_markup(GTK_LABEL(status_label), TRUE);
    gtk_widget_set_margin_start(status_label, 10);
    gtk_widget_set_margin_end(status_label, 10);
    gtk_widget_set_margin_top(status_label, 5);
    gtk_widget_set_margin_bottom(status_label, 5);
    gtk_box_append(GTK_BOX(main_box), status_label);

    // Live view area - takes most space
    GtkWidget *live_view = gtk_drawing_area_new();
    gtk_widget_set_vexpand(live_view, TRUE);
    gtk_widget_set_hexpand(live_view, TRUE);
    gtk_box_append(GTK_BOX(main_box), live_view);

    // Controls at bottom
    GtkWidget *controls = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_margin_start(controls, 10);
    gtk_widget_set_margin_end(controls, 10);
    gtk_widget_set_margin_top(controls, 5);
    gtk_widget_set_margin_bottom(controls, 5);
    gtk_box_append(GTK_BOX(main_box), controls);

    // Current cassette context
    GtkWidget *dir_label = gtk_label_new("Current Cassette Context:");
    GtkWidget *dir_entry = gtk_entry_new();
    gtk_editable_set_width_chars(GTK_EDITABLE(dir_entry), 30);
    gtk_box_append(GTK_BOX(controls), dir_label);
    gtk_box_append(GTK_BOX(controls), dir_entry);
    g_signal_connect(dir_entry, "changed", G_CALLBACK(on_directory_changed), NULL);

    // Context date
    GtkWidget *date_display_label = gtk_label_new("Context Date: none");
    gtk_label_set_use_markup(GTK_LABEL(date_display_label), TRUE);
    gtk_box_append(GTK_BOX(controls), date_display_label);

    GtkWidget *set_date_button = gtk_button_new_with_label("Set Date");
    GtkWidget *popover = gtk_popover_new();
    GtkWidget *calendar = gtk_calendar_new();
    gtk_popover_set_child(GTK_POPOVER(popover), calendar);
    gtk_widget_set_parent(popover, set_date_button);
    g_signal_connect(set_date_button, "clicked", G_CALLBACK(on_set_date_clicked), popover);
    g_signal_connect(calendar, "day-selected", G_CALLBACK(on_date_selected), date_display_label);
    gtk_box_append(GTK_BOX(controls), set_date_button);

    // Key controller for spacebar
    GtkEventController *key_controller = gtk_event_controller_key_new();
    g_signal_connect(key_controller, "key-pressed", G_CALLBACK(on_key_press), NULL);
    gtk_widget_add_controller(window, key_controller);

    // Initialize camera and live view
    camMgr = new CameraManager();
    if (camMgr->initialize() && camMgr->connectCamera() && camMgr->startLiveView()) {
        renderer = new LiveViewRenderer(live_view, camMgr);
        gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(live_view), draw_live_view, renderer, NULL);
        renderer->start();
        gtk_label_set_markup(GTK_LABEL(status_label), "Status: <span foreground='green'>Connected</span>");
    } else {
        gtk_label_set_markup(GTK_LABEL(status_label), "Status: <span foreground='red'>Failed to connect camera</span>");
    }

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.example.film", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    // Cleanup
    if (renderer) {
        renderer->stop();
        delete renderer;
    }
    if (camMgr) {
        delete camMgr;
    }

    return status;
}