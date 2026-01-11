#pragma once

#include <memory>
#include <string>
#include "EDSDKCompatibility.h"
#include "EDSDK.h"

class CameraManager {
public:
    CameraManager();
    ~CameraManager();
    
    bool initialize();
    bool connectToCamera();
    void disconnect();
    bool isConnected() const;
    
    EdsCameraRef getCameraRef() const { return m_camera; }
    
private:
    EdsCameraRef m_camera;
    EdsCameraListRef m_cameraList;
    bool m_initialized;
    bool m_connected;
    
    static EdsError EDSCALLBACK handleObjectEvent(EdsObjectEvent event, EdsBaseRef object, EdsVoid* context);
    static EdsError EDSCALLBACK handlePropertyEvent(EdsPropertyEvent event, EdsPropertyID property, EdsUInt32 data, EdsVoid* context);
    static EdsError EDSCALLBACK handleStateEvent(EdsStateEvent event, EdsUInt32 data, EdsVoid* context);
};