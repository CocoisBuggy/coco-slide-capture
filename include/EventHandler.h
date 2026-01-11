#pragma once

#include <gtk/gtk.h>
#include <functional>

class EventHandler {
public:
    EventHandler();
    ~EventHandler();
    
    void initialize(GtkWidget* widget);
    void setSpacebarCallback(std::function<void()> callback);
    
private:
    GtkWidget* m_widget;
    std::function<void()> m_spacebarCallback;
    
    static gboolean onKeyPress(GtkEventControllerKey* controller, guint keyval, guint keycode, GdkModifierType state, gpointer user_data);
};