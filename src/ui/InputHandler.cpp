#include "InputHandler.h"

#include <iostream>
#include <string>

#include "MainWindow.h"

gboolean InputHandler::on_key_press(GtkEventControllerKey *controller,
                                    guint keyval, guint keycode,
                                    GdkModifierType state, gpointer user_data) {
  MainWindow *self = static_cast<MainWindow *>(user_data);
  if (keyval == GDK_KEY_space) {
    std::cout << "Spacebar pressed: Trigger capture" << std::endl;
    if (self->camMgr && !self->active_directory.empty()) {
      // Get comment from entry field
      const char *comment_text =
          gtk_editable_get_text(GTK_EDITABLE(self->comment_entry));
      self->photo_comment = comment_text ? std::string(comment_text) : "";

      self->camMgr->capture(self->active_directory, self->photo_comment,
                            self->star_rating, self->renderer);

      // Reset comment field and star rating after capture
      gtk_editable_set_text(GTK_EDITABLE(self->comment_entry), "");
      self->photo_comment = "";
      self->star_rating = 0;
      update_star_display(self->star_rating_widget, 0);
    } else {
      std::cout << "Camera not connected or no active directory set"
                << std::endl;
    }
    return TRUE;
  } else if (keyval == GDK_KEY_n && (state & GDK_CONTROL_MASK)) {
    std::cout << "Ctrl+N pressed: Add Batch" << std::endl;
    // Will need to import this later
    // on_add_batch_clicked(GTK_BUTTON(self->batch_button), self);
    return TRUE;
  } else if (keyval == GDK_KEY_d && (state & GDK_CONTROL_MASK)) {
    std::cout << "Ctrl+D pressed: Set Date" << std::endl;
    // Will need to import this later
    // on_set_date_clicked(GTK_BUTTON(self->set_date_button), self->popover);
    return TRUE;
  } else if (keyval >= GDK_KEY_0 && keyval <= GDK_KEY_5) {
    // Star rating input (0-5)
    int rating = keyval - GDK_KEY_0;
    self->star_rating = rating;
    update_star_display(self->star_rating_widget, rating);
    std::cout << "Star rating set to: " << rating << std::endl;
    return TRUE;
  }
  return FALSE;
}

void InputHandler::update_star_display(GtkWidget *star_widget, int rating) {
  // Update star buttons to reflect current rating
  GtkWidget *parent = star_widget;
  GtkWidget *child = gtk_widget_get_first_child(parent);

  // Skip label and get to first star
  child = gtk_widget_get_next_sibling(child);

  int i = 0;
  while (child && i < 5) {
    gtk_button_set_label(GTK_BUTTON(child), (i < rating) ? "★" : "☆");

    // Update CSS class for color
    if (i < rating) {
      gtk_widget_remove_css_class(child, "star-empty");
      gtk_widget_add_css_class(child, "star-filled");
    } else {
      gtk_widget_remove_css_class(child, "star-filled");
      gtk_widget_add_css_class(child, "star-empty");
    }

    child = gtk_widget_get_next_sibling(child);
    i++;
  }
}