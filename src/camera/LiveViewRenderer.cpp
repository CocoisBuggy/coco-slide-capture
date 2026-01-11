#include "camera/LiveViewRenderer.h"

#include <gdk-pixbuf/gdk-pixbuf.h>

#include <chrono>
#include <iostream>
#include <thread>

LiveViewRenderer::LiveViewRenderer(GtkWidget* drawingArea,
                                   CameraManager* camMgr)
    : area(drawingArea),
      cameraMgr(camMgr),
      running(false),
      currentImage(NULL) {}

LiveViewRenderer::~LiveViewRenderer() {
  stop();
  if (currentImage) g_object_unref(currentImage);
}

void LiveViewRenderer::start() {
  if (running) return;
  running = true;
  liveThread = std::thread(&LiveViewRenderer::liveViewLoop, this);
}

void LiveViewRenderer::stop() {
  if (!running) return;
  running = false;
  if (liveThread.joinable()) liveThread.join();
}

void LiveViewRenderer::liveViewLoop() {
  while (running) {
    EdsStreamRef stream = NULL;
    EdsEvfImageRef evfImage = NULL;
    EdsError err = EdsCreateMemoryStream(0, &stream);
    if (err == EDS_ERR_OK) {
      err = EdsCreateEvfImageRef(stream, &evfImage);
      if (err == EDS_ERR_OK) {
        err = EdsDownloadEvfImage(cameraMgr->getCamera(), evfImage);
        if (err != EDS_ERR_OBJECT_NOTREADY && err != EDS_ERR_OK) {
          std::cout << "Download err: " << err << std::endl;
        }

        if (err == EDS_ERR_OK) {
          EdsUInt64 size = 0;
          EdsGetLength(stream, &size);
          EdsVoid* data = NULL;
          EdsGetPointer(stream, &data);

          if (data && size > 0) {
            GdkPixbufLoader* loader = gdk_pixbuf_loader_new();
            gdk_pixbuf_loader_write(loader, (const guchar*)data, size, NULL);
            gdk_pixbuf_loader_close(loader, NULL);
            GdkPixbuf* pixbuf = gdk_pixbuf_loader_get_pixbuf(loader);
            if (pixbuf) {
              g_object_ref(pixbuf);
              // Update currentImage in main thread
              g_idle_add(updateImage, this);
              // For simplicity, assume single thread access
              if (currentImage) g_object_unref(currentImage);
              currentImage = pixbuf;
            } else {
              std::cout << "No pixbuf" << std::endl;
            }
            g_object_unref(loader);
          }
        }
        EdsRelease(evfImage);
      }
      EdsRelease(stream);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

gboolean LiveViewRenderer::updateImage(gpointer data) {
  LiveViewRenderer* self = static_cast<LiveViewRenderer*>(data);
  gtk_widget_queue_draw(self->area);
  return FALSE;
}
