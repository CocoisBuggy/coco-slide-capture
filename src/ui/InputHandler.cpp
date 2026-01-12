#include "ui/InputHandler.h"

#include <iostream>
#include <string>

#include "ui/MainWindow.h"

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
                            self->renderer);

      // Reset comment field after capture
      gtk_editable_set_text(GTK_EDITABLE(self->comment_entry), "");
      self->photo_comment = "";
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
  }
  return FALSE;
}