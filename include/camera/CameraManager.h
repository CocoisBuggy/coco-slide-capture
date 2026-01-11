#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <EDSDK.h>
#include <string>

class CameraManager {
public:
    CameraManager();
    ~CameraManager();

    bool initialize();
    bool connectCamera();
    EdsCameraRef getCamera() const { return camera; }
    bool startLiveView();
    bool stopLiveView();
    EdsError downloadLiveViewImage(EdsStreamRef* stream);
    bool capture(const std::string& directory);

private:
    EdsCameraRef camera;
    bool isInitialized;
    std::string captureDirectory;

    static EdsError EDSCALLBACK objectEventHandler(EdsObjectEvent event, EdsBaseRef object, EdsVoid* context);
    void downloadImage(EdsBaseRef object);
};

#endif // CAMERAMANAGER_H