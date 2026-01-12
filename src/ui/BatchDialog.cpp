#include "ui/BatchDialog.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>

#include "ui/MainWindow.h"

void BatchDialog::on_add_batch_clicked(GtkButton *button, gpointer user_data) {
  MainWindow *self = static_cast<MainWindow *>(user_data);

  // Create popover
  GtkWidget *popover = gtk_popover_new();
  gtk_widget_set_parent(popover, self->batch_button);

  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_popover_set_child(GTK_POPOVER(popover), box);

  // Name
  GtkWidget *name_label = gtk_label_new("Name:");
  GtkWidget *name_entry = gtk_entry_new();
  gtk_box_append(GTK_BOX(box), name_label);
  gtk_box_append(GTK_BOX(box), name_entry);

  // Date
  GtkWidget *date_label = gtk_label_new("Date:");
  GtkWidget *date_cal = gtk_calendar_new();
  gtk_box_append(GTK_BOX(box), date_label);
  gtk_box_append(GTK_BOX(box), date_cal);

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
  g_signal_connect(name_entry, "activate", G_CALLBACK(on_entry_activate), data);
  g_signal_connect(cancel_button, "clicked", G_CALLBACK(on_dialog_cancel),
                   data);

  gtk_popover_popup(GTK_POPOVER(popover));
}

void BatchDialog::process_batch_data(DialogData *data, bool include_date) {
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

    if (include_date) {
      // Get date
      GDateTime *date = gtk_calendar_get_date(GTK_CALENDAR(data->date_cal));
      if (date) {
        char *date_str = g_date_time_format(date, "%Y-%m-%d");
        data->self->context_date = date_str;
        char *label_text = g_strdup_printf("Context Date: <b>%s</b>", date_str);
        gtk_label_set_markup(GTK_LABEL(data->self->date_display_label),
                             label_text);
        g_free(date_str);
        g_free(label_text);
        g_date_time_unref(date);
      }
    } else {
      // Set date to empty
      data->self->context_date = "";
      char *label_text = g_strdup_printf("Context Date: <b>%s</b>", "");
      gtk_label_set_markup(GTK_LABEL(data->self->date_display_label),
                           label_text);
      g_free(label_text);
    }

    data->self->active_directory = data->self->batch_name;
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

void BatchDialog::on_dialog_ok(GtkButton *button, gpointer user_data) {
  DialogData *data = static_cast<DialogData *>(user_data);
  process_batch_data(data, true);
}

void BatchDialog::on_entry_activate(GtkEntry *entry, gpointer user_data) {
  DialogData *data = static_cast<DialogData *>(user_data);
  process_batch_data(data, false);
}

void BatchDialog::on_dialog_cancel(GtkButton *button, gpointer user_data) {
  DialogData *data = static_cast<DialogData *>(user_data);
  gtk_popover_popdown(GTK_POPOVER(data->popover));
  delete data;
}