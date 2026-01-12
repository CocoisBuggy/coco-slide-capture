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
      paused(false),
      currentImage(NULL) {}

LiveViewRenderer::~LiveViewRenderer() {
  stop();
  std::lock_guard<std::mutex> lock(imageMutex);
  if (currentImage) g_object_unref(currentImage);
}

void LiveViewRenderer::start() {
  if (running) return;
  running = true;
  paused = false;
  liveThread = std::thread(&LiveViewRenderer::liveViewLoop, this);
}

void LiveViewRenderer::stop() {
  if (!running) return;
  running = false;
  paused = false;
  if (liveThread.joinable()) liveThread.join();
}

GdkPixbuf* LiveViewRenderer::getCurrentImage() const {
  std::lock_guard<std::mutex> lock(imageMutex);
  if (currentImage) {
    g_object_ref(currentImage);
    return currentImage;
  }
  return NULL;
}

void LiveViewRenderer::pauseForCapture() {
  paused = true;
  std::cout << "Live view paused for capture" << std::endl;
}

void LiveViewRenderer::resumeAfterCapture() {
  paused = false;
  std::cout << "Live view resumed after capture" << std::endl;
}

void LiveViewRenderer::liveViewLoop() {
  int consecutiveErrors = 0;
  const int maxErrors = 5;

  while (running) {
    // Always process SDK events, even when paused
    EdsGetEvent();

    // If paused for capture, just wait and continue
    if (paused) {
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      consecutiveErrors = 0;  // Reset error count when paused
      continue;
    }

    // Try to download live view image with timeout
    if (downloadLiveViewImage()) {
      consecutiveErrors = 0;  // Reset on success
    } else {
      consecutiveErrors++;
      std::cout << "Live view download failed (attempt " << consecutiveErrors
                << "/" << maxErrors << ")" << std::endl;

      // If we have too many consecutive errors, wait longer before retrying
      if (consecutiveErrors >= maxErrors) {
        std::cout << "Too many live view errors, waiting 2 seconds before retry"
                  << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        consecutiveErrors = 0;  // Reset after long wait
      } else {
        // Exponential backoff for errors
        int waitTime = 100 * (1 << std::min(consecutiveErrors - 1, 3));
        std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
      }
      continue;  // Skip the normal sleep
    }

    // Normal refresh rate when successful
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

bool LiveViewRenderer::downloadLiveViewImage() {
  EdsStreamRef stream = NULL;
  EdsEvfImageRef evfImage = NULL;
  bool success = false;

  // Set a timeout for the download operation
  auto startTime = std::chrono::steady_clock::now();
  const auto timeout = std::chrono::milliseconds(2000);  // 2 second timeout

  EdsError err = EdsCreateMemoryStream(0, &stream);
  if (err != EDS_ERR_OK) return false;

  err = EdsCreateEvfImageRef(stream, &evfImage);
  if (err != EDS_ERR_OK) {
    EdsRelease(stream);
    return false;
  }

  // Download with timeout check
  err = EdsDownloadEvfImage(cameraMgr->getCamera(), evfImage);

  // Check if we timed out
  auto elapsed = std::chrono::steady_clock::now() - startTime;
  if (elapsed > timeout) {
    std::cout << "Live view download timeout" << std::endl;
    EdsRelease(evfImage);
    EdsRelease(stream);
    return false;
  }

  if (err == EDS_ERR_OBJECT_NOTREADY) {
    // Camera is busy, this is normal
    EdsRelease(evfImage);
    EdsRelease(stream);
    return false;
  } else if (err != EDS_ERR_OK) {
    std::cout << "Live view download error: " << err << std::endl;
    EdsRelease(evfImage);
    EdsRelease(stream);
    return false;
  }

  // Successfully downloaded, process the image
  EdsUInt64 size = 0;
  EdsGetLength(stream, &size);
  EdsVoid* data = NULL;
  EdsGetPointer(stream, &data);

  if (data && size > 0) {
    GdkPixbufLoader* loader = gdk_pixbuf_loader_new();
    if (loader) {
      GError* error = NULL;
      gboolean write_ok =
          gdk_pixbuf_loader_write(loader, (const guchar*)data, size, &error);
      gboolean close_ok = gdk_pixbuf_loader_close(loader, &error);

      if (write_ok && close_ok) {
        GdkPixbuf* pixbuf = gdk_pixbuf_loader_get_pixbuf(loader);
        if (pixbuf) {
          g_object_ref(pixbuf);
          updateCurrentImage(pixbuf);
          success = true;
        }
      } else if (error) {
        std::cout << "Pixbuf loader error: " << error->message << std::endl;
        g_error_free(error);
      }
      g_object_unref(loader);
    }
  }

  EdsRelease(evfImage);
  EdsRelease(stream);
  return success;
}

void LiveViewRenderer::updateCurrentImage(GdkPixbuf* newImage) {
  std::lock_guard<std::mutex> lock(imageMutex);
  if (currentImage) g_object_unref(currentImage);
  currentImage = newImage;

  // Schedule UI update in main thread
  g_idle_add(updateImage, this);
}

gboolean LiveViewRenderer::updateImage(gpointer data) {
  LiveViewRenderer* self = static_cast<LiveViewRenderer*>(data);
  gtk_widget_queue_draw(self->area);
  return FALSE;
}
