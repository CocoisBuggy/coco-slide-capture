#include "ui/UIBuilder.h"

#include <iostream>

#include "ui/BatchDialog.h"
#include "ui/InputHandler.h"
#include "ui/LiveViewDrawer.h"
#include "ui/MainWindow.h"

GtkWidget *UIBuilder::create_main_window(GtkApplication *app,
                                         MainWindow *mainWindow) {
  GtkWidget *window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Canon Camera Control");
  gtk_window_set_default_size(GTK_WINDOW(window), 1200, 800);

  // Create main vertical box
  GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_window_set_child(GTK_WINDOW(window), main_box);

  // Status label at top
  GtkWidget *status_label = create_status_label();
  gtk_box_append(GTK_BOX(main_box), status_label);

  // Live view area - takes most space
  GtkWidget *live_view = create_live_view_area();
  gtk_box_append(GTK_BOX(main_box), live_view);

  // Controls at bottom
  GtkWidget *controls = create_controls(mainWindow);
  gtk_box_append(GTK_BOX(main_box), controls);

  // Store widget references in MainWindow
  mainWindow->window = window;
  mainWindow->status_label = status_label;
  mainWindow->live_view = live_view;

  return window;
}

GtkWidget *UIBuilder::create_status_label() {
  GtkWidget *status_label = gtk_label_new("Status: Initializing");
  gtk_label_set_use_markup(GTK_LABEL(status_label), TRUE);
  gtk_widget_set_margin_start(status_label, 10);
  gtk_widget_set_margin_end(status_label, 10);
  gtk_widget_set_margin_top(status_label, 5);
  gtk_widget_set_margin_bottom(status_label, 5);
  return status_label;
}

GtkWidget *UIBuilder::create_live_view_area() {
  GtkWidget *live_view = gtk_drawing_area_new();
  gtk_widget_set_vexpand(live_view, TRUE);
  gtk_widget_set_hexpand(live_view, TRUE);
  return live_view;
}

GtkWidget *UIBuilder::create_controls(MainWindow *mainWindow) {
  GtkWidget *controls = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_widget_set_margin_start(controls, 10);
  gtk_widget_set_margin_end(controls, 10);
  gtk_widget_set_margin_top(controls, 5);
  gtk_widget_set_margin_bottom(controls, 5);

  // Current batch
  GtkWidget *batch_label = gtk_label_new("Current Batch:");
  GtkWidget *batch_button = gtk_button_new_with_label("Add Batch");
  GtkWidget *batch_display_label = gtk_label_new("Current Batch: none");
  gtk_label_set_use_markup(GTK_LABEL(batch_display_label), TRUE);
  gtk_box_append(GTK_BOX(controls), batch_label);
  gtk_box_append(GTK_BOX(controls), batch_display_label);
  gtk_box_append(GTK_BOX(controls), batch_button);

  // Store references
  mainWindow->batch_button = batch_button;
  mainWindow->batch_display_label = batch_display_label;

  // Context date
  GtkWidget *date_display_label = gtk_label_new("Context Date: none");
  gtk_label_set_use_markup(GTK_LABEL(date_display_label), TRUE);
  gtk_box_append(GTK_BOX(controls), date_display_label);
  mainWindow->date_display_label = date_display_label;

  GtkWidget *set_date_button = gtk_button_new_with_label("Set Date");
  GtkWidget *popover = gtk_popover_new();
  GtkWidget *calendar = gtk_calendar_new();
  gtk_popover_set_child(GTK_POPOVER(popover), calendar);
  gtk_widget_set_parent(popover, set_date_button);
  gtk_box_append(GTK_BOX(controls), set_date_button);

  // Store references
  mainWindow->set_date_button = set_date_button;
  mainWindow->popover = popover;
  mainWindow->calendar = calendar;

  // Photo comment field
  GtkWidget *comment_label = gtk_label_new("Comment:");
  GtkWidget *comment_entry = gtk_entry_new();
  gtk_widget_set_hexpand(comment_entry, TRUE);
  gtk_box_append(GTK_BOX(controls), comment_label);
  gtk_box_append(GTK_BOX(controls), comment_entry);
  mainWindow->comment_entry = comment_entry;

  return controls;
}

void UIBuilder::setup_signals(GtkWidget *window, GtkWidget *batch_button,
                              GtkWidget *set_date_button, GtkWidget *calendar,
                              MainWindow *mainWindow) {
  // Batch button signal
  g_signal_connect(batch_button, "clicked",
                   G_CALLBACK(BatchDialog::on_add_batch_clicked), mainWindow);

  // Set date button signal
  g_signal_connect(set_date_button, "clicked",
                   G_CALLBACK([](GtkButton *button, gpointer user_data) {
                     GtkPopover *popover = GTK_POPOVER(user_data);
                     gtk_popover_popup(popover);
                   }),
                   mainWindow->popover);

  // Calendar signal
  g_signal_connect(
      calendar, "day-selected",
      G_CALLBACK([](GtkCalendar *calendar, gpointer user_data) {
        MainWindow *self = static_cast<MainWindow *>(user_data);
        GDateTime *date = gtk_calendar_get_date(calendar);
        if (date) {
          char *str = g_date_time_format(date, "%Y-%m-%d");
          self->context_date = str;
          char *label_text = g_strdup_printf("Context Date: <b>%s</b>", str);
          gtk_label_set_markup(GTK_LABEL(self->date_display_label), label_text);
          std::cout << "Context date set to: " << self->context_date
                    << std::endl;
          g_free(str);
          g_free(label_text);
          g_date_time_unref(date);
        }
        // Hide popover
        GtkWidget *popover =
            gtk_widget_get_ancestor(GTK_WIDGET(calendar), GTK_TYPE_POPOVER);
        if (popover) gtk_popover_popdown(GTK_POPOVER(popover));
      }),
      mainWindow);

  // Key controller for spacebar
  GtkEventController *key_controller = gtk_event_controller_key_new();
  g_signal_connect(key_controller, "key-pressed",
                   G_CALLBACK(InputHandler::on_key_press), mainWindow);
  gtk_widget_add_controller(window, key_controller);

  // Window close handler for proper cleanup
  g_signal_connect(window, "close-request",
                   G_CALLBACK([](GtkWindow *window, gpointer user_data) {
                     MainWindow *self = static_cast<MainWindow *>(user_data);
                     std::cout << "Window close requested - performing cleanup"
                               << std::endl;

                     // Explicitly disconnect camera to ensure proper cleanup
                     if (self->camMgr) {
                       self->camMgr->disconnectCamera();
                     }

                     // Allow window to close
                     return FALSE;
                   }),
                   mainWindow);
}