#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <chrono>
#include <unistd.h>

#include "CameraManager.h"
#include "FileManager.h"

class Application {
public:
    Application();
    ~Application();
    
    int run();
    
private:
    std::unique_ptr<CameraManager> m_cameraManager;
    std::unique_ptr<FileManager> m_fileManager;
    
    bool m_running;
    
    void setupConsole();
    void onCapture();
    void updateStatus(const std::string& message);
    
    static void showHelp();
};

Application::Application()
    : m_running(false) {
    
    m_cameraManager = std::make_unique<CameraManager>();
    m_fileManager = std::make_unique<FileManager>();
}

Application::~Application() {
}

int Application::run() {
    setupConsole();
    
    updateStatus("Starting film scanner...");
    
    // Initialize camera
    if (!m_cameraManager->initialize()) {
        updateStatus("Failed to initialize EDSDK");
        return 1;
    }
    
    if (!m_cameraManager->connectToCamera()) {
        updateStatus("No camera found. Please connect a Canon camera and restart.");
        return 1;
    }
    
    updateStatus("Camera connected. Press SPACE to capture, 'q' to quit, 'h' for help.");
    m_running = true;
    
    // Simple console event loop
    std::thread inputThread([this]() {
        char input;
        while (m_running && std::cin.get(input)) {
            switch (input) {
                case ' ':
                    onCapture();
                    break;
                case 'q':
                case 'Q':
                    m_running = false;
                    break;
                case 'h':
                case 'H':
                    showHelp();
                    break;
                case '\n':
                    // Skip newlines
                    break;
                default:
                    std::cout << "Unknown command. Press 'h' for help.\n";
                    break;
            }
        }
    });
    
    // Main application loop
    while (m_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        // Here we would update live view in GUI version
        // For now, just keep the application running
    }
    
    inputThread.join();
    updateStatus("Application shutting down...");
    
    return 0;
}

void Application::setupConsole() {
    std::cout << "\n========================================\n";
    std::cout << "   Canon Film Scanner Console\n";
    std::cout << "========================================\n\n";
}

void Application::onCapture() {
    if (!m_cameraManager->isConnected()) {
        updateStatus("No camera connected");
        return;
    }
    
    updateStatus("Capturing...");
    
    // In real implementation, this would:
    // 1. Auto focus
    // 2. Auto expose  
    // 3. Take photo
    // 4. Download file via FileManager
    
    // For now, simulate capture
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    updateStatus("Capture simulated (capture logic needs EDSDK implementation)");
}

void Application::updateStatus(const std::string& message) {
    std::cout << "[STATUS] " << message << std::endl;
}

void Application::showHelp() {
    std::cout << "\n--- Help ---\n";
    std::cout << "SPACE - Capture photo\n";
    std::cout << "q     - Quit application\n";
    std::cout << "h     - Show this help\n";
    std::cout << "-------------\n\n";
}

int main(int argc, char* argv[]) {
    Application app;
    return app.run();
}