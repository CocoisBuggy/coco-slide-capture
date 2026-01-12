#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtk-4.0/gtk/gtk.h>

#include <string>

#include "camera/CameraManager.h"
#include "camera/LiveViewRenderer.h"

// Forward declarations for UI modules
class UIBuilder;
class LiveViewDrawer;
class InputHandler;
class BatchDialog;

class MainWindow {
 public:
  MainWindow(GtkApplication *app);
  ~MainWindow();

  void show();

  // Make UI components accessible to UIBuilder
  GtkWidget *window;
  GtkWidget *status_label;
  GtkWidget *live_view;
  GtkWidget *batch_button;
  GtkWidget *batch_display_label;
  GtkWidget *date_display_label;
  GtkWidget *set_date_button;
  GtkWidget *popover;
  GtkWidget *calendar;
  GtkWidget *comment_entry;

  // State accessible to other modules
  std::string active_directory;
  std::string context_date;
  std::string batch_name;
  std::string photo_comment;

  // Camera accessible to other modules
  CameraManager *camMgr;
  LiveViewRenderer *renderer;
};

#endif  // MAINWINDOW_H