#pragma once

#include <memory>
#include <string>
#include <gtk/gtk.h>

class LiveViewRenderer {
public:
    LiveViewRenderer();
    ~LiveViewRenderer();
    
    bool initialize();
    void updateFrame(const unsigned char* data, int width, int height);
    void render();
    void setWidget(GtkWidget* widget) { m_widget = widget; }
    
private:
    GtkWidget* m_widget;
    GdkPixbuf* m_pixbuf;
    unsigned char* m_frameData;
    int m_width;
    int m_height;
    bool m_initialized;
    
    void createPixbufIfNeeded();
};