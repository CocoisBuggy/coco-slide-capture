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

private:
    EdsCameraRef camera;
    bool isInitialized;
};

#endif // CAMERAMANAGER_H