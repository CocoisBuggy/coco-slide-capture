#include "UIBuilder.h"

#include <iostream>
#include <string>

#include "BatchDialog.h"
#include "InputHandler.h"
#include "LiveViewDrawer.h"
#include "MainWindow.h"

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

  // Star rating field
  GtkWidget *star_rating_widget = create_star_rating_widget(mainWindow);
  gtk_box_append(GTK_BOX(controls), star_rating_widget);
  mainWindow->star_rating_widget = star_rating_widget;

  return controls;
}

void UIBuilder::setup_signals(GtkWidget *window, GtkWidget *batch_button,
                              GtkWidget *set_date_button, GtkWidget *calendar,
                              MainWindow *mainWindow) {
  // Batch button signal
  g_signal_connect(batch_button, "clicked",
                   G_CALLBACK(BatchDialog::on_add_batch_clicked), mainWindow);

  // Set date button signal
  static auto on_set_date_clicked = +[](GtkButton *button, gpointer user_data) {
    GtkPopover *popover = GTK_POPOVER(user_data);
    gtk_popover_popup(popover);
  };
  g_signal_connect(set_date_button, "clicked", G_CALLBACK(on_set_date_clicked),
                   mainWindow->popover);

  // Calendar signal
  static auto on_day_selected = +[](GtkCalendar *calendar, gpointer user_data) {
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
  };
  g_signal_connect(calendar, "day-selected", G_CALLBACK(on_day_selected),
                   mainWindow);

  // Key controller for spacebar
  GtkEventController *key_controller = gtk_event_controller_key_new();
  g_signal_connect(key_controller, "key-pressed",
                   G_CALLBACK(InputHandler::on_key_press), mainWindow);
  gtk_widget_add_controller(window, key_controller);

  // Window close handler for proper cleanup
  static auto on_close_request = +[](GtkWindow *window, gpointer user_data) {
    MainWindow *self = static_cast<MainWindow *>(user_data);
    std::cout << "Window close requested - performing cleanup" << std::endl;

    // Explicitly disconnect camera to ensure proper cleanup
    if (self->camMgr) {
      self->camMgr->disconnectCamera();
    }

    // Allow window to close
    return FALSE;
  };
  g_signal_connect(window, "close-request", G_CALLBACK(on_close_request),
                   mainWindow);
}

GtkWidget *UIBuilder::create_star_rating_widget(MainWindow *mainWindow) {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

  // Apply CSS for yellow stars
  GtkCssProvider *css_provider = gtk_css_provider_new();
  const char *css =
      ".star-button { "
      "  font-size: 18px; "
      "  padding: 2px 4px; "
      "  margin: 0px; "
      "} "
      ".star-filled { "
      "  color: #FFD700; " /* Gold yellow color */
      "} "
      ".star-empty { "
      "  color: #D3D3D3; " /* Light gray for empty stars */
      "}";
  gtk_css_provider_load_from_string(css_provider, css);
  gtk_style_context_add_provider_for_display(
      gdk_display_get_default(), GTK_STYLE_PROVIDER(css_provider),
      GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  // Label
  GtkWidget *label = gtk_label_new("Rating:");
  gtk_box_append(GTK_BOX(box), label);

  // Create 5 star buttons
  for (int i = 0; i < 5; i++) {
    GtkWidget *star_button = gtk_button_new_with_label("☆");
    gtk_widget_set_name(star_button, ("star_" + std::to_string(i)).c_str());

    // Apply star button specific styling
    gtk_widget_add_css_class(star_button, "star-button");
    gtk_widget_add_css_class(star_button, "star-empty");

    // Store star index in button for click handler
    g_object_set_data(G_OBJECT(star_button), "star_index", GINT_TO_POINTER(i));

    // Click handler
    static auto on_star_clicked = +[](GtkButton *button, gpointer user_data) {
      MainWindow *self = static_cast<MainWindow *>(user_data);
      int star_index =
          GPOINTER_TO_INT(g_object_get_data(G_OBJECT(button), "star_index"));
      self->star_rating =
          star_index + 1;  // 1-based for user, 0-based internally

      // Update all star displays
      GtkWidget *parent = gtk_widget_get_parent(GTK_WIDGET(button));
      GtkWidget *child = gtk_widget_get_first_child(parent);

      // Skip -- label and get to the first star
      child = gtk_widget_get_next_sibling(child);

      int j = 0;
      while (child && j < 5) {
        gtk_button_set_label(GTK_BUTTON(child), (j <= star_index) ? "★" : "☆");

        // Update CSS class for color
        if (j <= star_index) {
          gtk_widget_remove_css_class(child, "star-empty");
          gtk_widget_add_css_class(child, "star-filled");
        } else {
          gtk_widget_remove_css_class(child, "star-filled");
          gtk_widget_add_css_class(child, "star-empty");
        }

        child = gtk_widget_get_next_sibling(child);
        j++;
      }
    };

    g_signal_connect(star_button, "clicked", G_CALLBACK(on_star_clicked),
                     mainWindow);
    gtk_box_append(GTK_BOX(box), star_button);
  }

  return box;
}