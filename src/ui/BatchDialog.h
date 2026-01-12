#pragma once

#include <gtk/gtk.h>

class MainWindow;

struct DialogData {
  MainWindow *self;
  GtkWidget *date_cal;
  GtkWidget *name_entry;
  GtkWidget *popover;
};

class BatchDialog {
 public:
  static void on_add_batch_clicked(GtkButton *button, gpointer user_data);
  static void on_dialog_ok(GtkButton *button, gpointer user_data);
  static void on_entry_activate(GtkEntry *entry, gpointer user_data);
  static void on_dialog_cancel(GtkButton *button, gpointer user_data);

 private:
  static void process_batch_data(DialogData *data, bool include_date);
};