#ifndef LIVEVIEWRENDERER_H
#define LIVEVIEWRENDERER_H

#include <gtk/gtk.h>
#include <thread>
#include <atomic>
#include "CameraManager.h"

class LiveViewRenderer {
public:
    LiveViewRenderer(GtkWidget* drawingArea, CameraManager* camMgr);
    ~LiveViewRenderer();

    void start();
    void stop();
    GdkPixbuf* getCurrentImage() const { return currentImage; }

private:
    GtkWidget* area;
    CameraManager* cameraMgr;
    std::thread liveThread;
    std::atomic<bool> running;
    GdkPixbuf* currentImage;

    void liveViewLoop();
    static gboolean updateImage(gpointer data);
};

#endif // LIVEVIEWRENDERER_H