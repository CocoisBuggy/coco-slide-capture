#include "LiveViewRenderer.h"
#include <iostream>

LiveViewRenderer::LiveViewRenderer()
    : m_widget(nullptr)
    , m_pixbuf(nullptr)
    , m_frameData(nullptr)
    , m_width(0)
    , m_height(0)
    , m_initialized(false) {
}

LiveViewRenderer::~LiveViewRenderer() {
    if (m_pixbuf) {
        g_object_unref(m_pixbuf);
        m_pixbuf = nullptr;
    }
    
    if (m_frameData) {
        delete[] m_frameData;
        m_frameData = nullptr;
    }
}

bool LiveViewRenderer::initialize() {
    m_initialized = true;
    return true;
}

void LiveViewRenderer::updateFrame(const unsigned char* data, int width, int height) {
    if (!m_initialized || !m_widget) {
        return;
    }
    
    // Reallocate frame data if size changed
    if (width != m_width || height != m_height) {
        if (m_frameData) {
            delete[] m_frameData;
        }
        
        m_width = width;
        m_height = height;
        m_frameData = new unsigned char[width * height * 3]; // RGB
        
        // Clear old pixbuf
        if (m_pixbuf) {
            g_object_unref(m_pixbuf);
            m_pixbuf = nullptr;
        }
    }
    
    // Convert from camera format to RGB (simplified conversion)
    for (int i = 0; i < width * height; i++) {
        m_frameData[i * 3 + 0] = data[i * 2];     // R
        m_frameData[i * 3 + 1] = data[i * 2];     // G
        m_frameData[i * 3 + 2] = data[i * 2];     // B
    }
    
    createPixbufIfNeeded();
    render();
}

void LiveViewRenderer::render() {
    if (!m_widget || !m_pixbuf) {
        return;
    }
    
    gtk_image_set_from_pixbuf(GTK_IMAGE(m_widget), m_pixbuf);
}

void LiveViewRenderer::createPixbufIfNeeded() {
    if (m_pixbuf || !m_frameData) {
        return;
    }
    
    m_pixbuf = gdk_pixbuf_new_from_data(
        m_frameData,
        GDK_COLORSPACE_RGB,
        FALSE,
        8,
        m_width,
        m_height,
        m_width * 3,
        nullptr,
        nullptr
    );
}