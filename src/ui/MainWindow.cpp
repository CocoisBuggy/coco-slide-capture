#include "ui/MainWindow.h"

#include <cairo.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>

// Callback for drawing the live view
void MainWindow::draw_live_view(GtkDrawingArea *area, cairo_t *cr, int width,
                                int height, gpointer data) {
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

// Callback for key press
gboolean MainWindow::on_key_press(GtkEventControllerKey *controller,
                                  guint keyval, guint keycode,
                                  GdkModifierType state, gpointer user_data) {
  MainWindow *self = static_cast<MainWindow *>(user_data);
  if (keyval == GDK_KEY_space) {
    std::cout << "Spacebar pressed: Trigger capture" << std::endl;
    if (self->camMgr && !self->active_directory.empty()) {
      self->camMgr->capture(self->active_directory);
    } else {
      std::cout << "Camera not connected or no active directory set"
                << std::endl;
    }
    return TRUE;
  }
  return FALSE;
}

// Callback for set date button
void MainWindow::on_set_date_clicked(GtkButton *button, gpointer user_data) {
  GtkPopover *popover = GTK_POPOVER(user_data);
  gtk_popover_popup(popover);
}

// Callback for date selected
void MainWindow::on_date_selected(GtkCalendar *calendar, gpointer user_data) {
  MainWindow *self = static_cast<MainWindow *>(user_data);
  GDateTime *date = gtk_calendar_get_date(calendar);
  if (date) {
    char *str = g_date_time_format(date, "%Y-%m-%d");
    self->context_date = str;
    char *label_text = g_strdup_printf("Context Date: <b>%s</b>", str);
    gtk_label_set_markup(GTK_LABEL(self->date_display_label), label_text);
    std::cout << "Context date set to: " << self->context_date << std::endl;
    g_free(str);
    g_free(label_text);
    g_date_time_unref(date);
  }
  // Hide popover
  GtkWidget *popover =
      gtk_widget_get_ancestor(GTK_WIDGET(calendar), GTK_TYPE_POPOVER);
  if (popover) gtk_popover_popdown(GTK_POPOVER(popover));
}

// Callback for add batch button
void MainWindow::on_add_batch_clicked(GtkButton *button, gpointer user_data) {
  MainWindow *self = static_cast<MainWindow *>(user_data);

  // Create popover
  GtkWidget *popover = gtk_popover_new();
  gtk_widget_set_parent(popover, self->batch_button);

  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_popover_set_child(GTK_POPOVER(popover), box);

  // Date
  GtkWidget *date_label = gtk_label_new("Date:");
  GtkWidget *date_cal = gtk_calendar_new();
  gtk_box_append(GTK_BOX(box), date_label);
  gtk_box_append(GTK_BOX(box), date_cal);

  // Name
  GtkWidget *name_label = gtk_label_new("Name:");
  GtkWidget *name_entry = gtk_entry_new();
  gtk_box_append(GTK_BOX(box), name_label);
  gtk_box_append(GTK_BOX(box), name_entry);

  // Buttons
  GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_box_append(GTK_BOX(box), button_box);
  GtkWidget *ok_button = gtk_button_new_with_label("OK");
  GtkWidget *cancel_button = gtk_button_new_with_label("Cancel");
  gtk_box_append(GTK_BOX(button_box), cancel_button);
  gtk_box_append(GTK_BOX(button_box), ok_button);

  // Data
  DialogData *data = new DialogData{self, date_cal, name_entry, popover};

  g_signal_connect(ok_button, "clicked", G_CALLBACK(on_dialog_ok), data);
  g_signal_connect(cancel_button, "clicked", G_CALLBACK(on_dialog_cancel),
                   data);

  gtk_popover_popup(GTK_POPOVER(popover));
}

// Callback for dialog OK
void MainWindow::on_dialog_ok(GtkButton *button, gpointer user_data) {
  DialogData *data = static_cast<DialogData *>(user_data);

  // Get date
  GDateTime *date = gtk_calendar_get_date(GTK_CALENDAR(data->date_cal));
  if (date) {
    char *date_str = g_date_time_format(date, "%Y-%m-%d");
    data->self->context_date = date_str;
    char *label_text = g_strdup_printf("Context Date: <b>%s</b>", date_str);
    gtk_label_set_markup(GTK_LABEL(data->self->date_display_label), label_text);
    g_free(date_str);
    g_free(label_text);
    g_date_time_unref(date);
  }

  // Get name
  const char *name_text = gtk_editable_get_text(GTK_EDITABLE(data->name_entry));
  std::string name = name_text;
  // Trim
  size_t start = name.find_first_not_of(" \t");
  if (start != std::string::npos) {
    size_t end = name.find_last_not_of(" \t");
    name = name.substr(start, end - start + 1);
  } else {
    name = "";
  }
  if (!name.empty() && std::all_of(name.begin(), name.end(),
                                   [](char c) { return std::isprint(c); })) {
    data->self->batch_name = name;
    data->self->active_directory =
        data->self->context_date + "/" + data->self->batch_name;
    char *batch_text = g_strdup_printf("Current Batch: <b>%s</b>",
                                       data->self->batch_name.c_str());
    gtk_label_set_markup(GTK_LABEL(data->self->batch_display_label),
                         batch_text);
    std::cout << "Batch added: " << data->self->active_directory << std::endl;
    g_free(batch_text);
  } else {
    std::cout << "Invalid batch name" << std::endl;
  }

  gtk_popover_popdown(GTK_POPOVER(data->popover));
  delete data;
}

// Callback for dialog cancel
void MainWindow::on_dialog_cancel(GtkButton *button, gpointer user_data) {
  DialogData *data = static_cast<DialogData *>(user_data);
  gtk_popover_popdown(GTK_POPOVER(data->popover));
  delete data;
}

MainWindow::MainWindow(GtkApplication *app)
    : camMgr(nullptr), renderer(nullptr) {
  // Create window
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Canon Camera Control");
  gtk_window_set_default_size(GTK_WINDOW(window), 1200, 800);

  // Create main vertical box
  GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_window_set_child(GTK_WINDOW(window), main_box);

  // Status label at top
  status_label = gtk_label_new("Status: Initializing");
  gtk_label_set_use_markup(GTK_LABEL(status_label), TRUE);
  gtk_widget_set_margin_start(status_label, 10);
  gtk_widget_set_margin_end(status_label, 10);
  gtk_widget_set_margin_top(status_label, 5);
  gtk_widget_set_margin_bottom(status_label, 5);
  gtk_box_append(GTK_BOX(main_box), status_label);

  // Live view area - takes most space
  live_view = gtk_drawing_area_new();
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

  // Current batch
  GtkWidget *batch_label = gtk_label_new("Current Batch:");
  batch_button = gtk_button_new_with_label("Add Batch");
  batch_display_label = gtk_label_new("Current Batch: none");
  gtk_label_set_use_markup(GTK_LABEL(batch_display_label), TRUE);
  gtk_box_append(GTK_BOX(controls), batch_label);
  gtk_box_append(GTK_BOX(controls), batch_display_label);
  gtk_box_append(GTK_BOX(controls), batch_button);
  g_signal_connect(batch_button, "clicked", G_CALLBACK(on_add_batch_clicked),
                   this);

  // Context date
  date_display_label = gtk_label_new("Context Date: none");
  gtk_label_set_use_markup(GTK_LABEL(date_display_label), TRUE);
  gtk_box_append(GTK_BOX(controls), date_display_label);

  set_date_button = gtk_button_new_with_label("Set Date");
  popover = gtk_popover_new();
  calendar = gtk_calendar_new();
  gtk_popover_set_child(GTK_POPOVER(popover), calendar);
  gtk_widget_set_parent(popover, set_date_button);
  g_signal_connect(set_date_button, "clicked", G_CALLBACK(on_set_date_clicked),
                   popover);
  g_signal_connect(calendar, "day-selected", G_CALLBACK(on_date_selected),
                   this);
  gtk_box_append(GTK_BOX(controls), set_date_button);

  // Key controller for spacebar
  GtkEventController *key_controller = gtk_event_controller_key_new();
  g_signal_connect(key_controller, "key-pressed", G_CALLBACK(on_key_press),
                   this);
  gtk_widget_add_controller(window, key_controller);

  // Initialize camera and live view
  camMgr = new CameraManager();
  if (camMgr->initialize() && camMgr->connectCamera() &&
      camMgr->startLiveView()) {
    renderer = new LiveViewRenderer(live_view, camMgr);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(live_view), draw_live_view,
                                   this, NULL);
    renderer->start();
    gtk_label_set_markup(GTK_LABEL(status_label),
                         "Status: <span foreground='green'>Connected</span>");
  } else {
    gtk_label_set_markup(
        GTK_LABEL(status_label),
        "Status: <span foreground='red'>Failed to connect camera</span>");
  }
}

MainWindow::~MainWindow() {
  if (renderer) {
    renderer->stop();
    delete renderer;
  }
  if (camMgr) {
    delete camMgr;
  }
}

void MainWindow::show() { gtk_window_present(GTK_WINDOW(window)); }
