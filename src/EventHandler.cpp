#include "EventHandler.h"
#include <iostream>

EventHandler::EventHandler()
    : m_widget(nullptr) {
}

EventHandler::~EventHandler() {
}

void EventHandler::initialize(GtkWidget* widget) {
    m_widget = widget;
    
    // Create key event controller
    GtkEventController* controller = gtk_event_controller_key_new();
    g_signal_connect(controller, "key-pressed", G_CALLBACK(onKeyPress), this);
    gtk_widget_add_controller(m_widget, controller);
}

void EventHandler::setSpacebarCallback(std::function<void()> callback) {
    m_spacebarCallback = callback;
}

gboolean EventHandler::onKeyPress(GtkEventControllerKey* controller, guint keyval, guint keycode, GdkModifierType state, gpointer user_data) {
    EventHandler* self = static_cast<EventHandler*>(user_data);
    
    if (keyval == GDK_KEY_space) {
        if (self->m_spacebarCallback) {
            self->m_spacebarCallback();
        }
        return TRUE; // Handled
    }
    
    return FALSE; // Not handled
}