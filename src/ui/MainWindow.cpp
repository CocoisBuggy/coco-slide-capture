#include "ui/MainWindow.h"

#include "LiveViewDrawer.h"
#include "UIBuilder.h"
#include "ui/UIBuilder.h"

MainWindow::MainWindow(GtkApplication *app)
    : camMgr(nullptr), renderer(nullptr) {
  // Create UI using UIBuilder
  UIBuilder::create_main_window(app, this);

  // Setup signals
  UIBuilder::setup_signals(window, batch_button, set_date_button, calendar,
                           this);

  // Initialize camera and live view
  camMgr = new CameraManager();
  if (camMgr->initialize() && camMgr->connectCamera() &&
      camMgr->startLiveView()) {
    renderer = new LiveViewRenderer(live_view, camMgr);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(live_view),
                                   LiveViewDrawer::draw_live_view, this, NULL);
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
