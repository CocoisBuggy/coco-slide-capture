#ifndef LIVEVIEWRENDERER_H
#define LIVEVIEWRENDERER_H

#include <gtk/gtk.h>

#include <atomic>
#include <mutex>
#include <thread>

#include "CameraManager.h"

class LiveViewRenderer {
 public:
  LiveViewRenderer(GtkWidget* drawingArea, CameraManager* camMgr);
  ~LiveViewRenderer();

  void start();
  void stop();
  GdkPixbuf* getCurrentImage() const;

  // Called by CameraManager during capture to prevent conflicts
  void pauseForCapture();
  void resumeAfterCapture();

 private:
  GtkWidget* area;
  CameraManager* cameraMgr;
  std::thread liveThread;
  std::atomic<bool> running;
  std::atomic<bool> paused;
  GdkPixbuf* currentImage;
  mutable std::mutex imageMutex;

  void liveViewLoop();
  static gboolean updateImage(gpointer data);
  bool downloadLiveViewImage();
  void updateCurrentImage(GdkPixbuf* newImage);
};

#endif  // LIVEVIEWRENDERER_H