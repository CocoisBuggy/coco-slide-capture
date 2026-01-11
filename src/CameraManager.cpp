#include "CameraManager.h"
#include <iostream>

CameraManager::CameraManager() : camera(NULL), isInitialized(false) {}

CameraManager::~CameraManager() {
    if (camera) {
        EdsRelease(camera);
    }
    if (isInitialized) {
        EdsTerminateSDK();
    }
}

bool CameraManager::initialize() {
    EdsError err = EdsInitializeSDK();
    if (err == EDS_ERR_OK) {
        isInitialized = true;
        std::cout << "EDSDK initialized" << std::endl;
        return true;
    } else {
        std::cout << "Failed to initialize EDSDK: " << err << std::endl;
        return false;
    }
}

bool CameraManager::connectCamera() {
    EdsCameraListRef cameraList = NULL;
    EdsError err = EdsGetCameraList(&cameraList);
    if (err != EDS_ERR_OK) {
        std::cout << "Failed to get camera list: " << err << std::endl;
        return false;
    }

    EdsUInt32 count = 0;
    err = EdsGetChildCount(cameraList, &count);
    if (err != EDS_ERR_OK || count == 0) {
        std::cout << "No cameras found" << std::endl;
        EdsRelease(cameraList);
        return false;
    }

    err = EdsGetChildAtIndex(cameraList, 0, &camera);
    EdsRelease(cameraList);
    if (err != EDS_ERR_OK) {
        std::cout << "Failed to get camera: " << err << std::endl;
        return false;
    }

    err = EdsOpenSession(camera);
    if (err != EDS_ERR_OK) {
        std::cout << "Failed to open session: " << err << std::endl;
        EdsRelease(camera);
        camera = NULL;
        return false;
    }

    std::cout << "Camera connected" << std::endl;
    return true;
}

bool CameraManager::startLiveView() {
    if (!camera) return false;
    EdsUInt32 device = kEdsEvfOutputDevice_PC;
    EdsError err = EdsSetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(EdsUInt32), &device);
    if (err != EDS_ERR_OK) {
        std::cout << "Failed to start live view: " << err << std::endl;
        return false;
    }
    std::cout << "Live view started" << std::endl;
    return true;
}

bool CameraManager::stopLiveView() {
    if (!camera) return false;
    EdsUInt32 device = 0;
    EdsError err = EdsSetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(EdsUInt32), &device);
    if (err != EDS_ERR_OK) {
        std::cout << "Failed to stop live view: " << err << std::endl;
        return false;
    }
    std::cout << "Live view stopped" << std::endl;
    return true;
}

EdsError CameraManager::downloadLiveViewImage(EdsStreamRef* stream) {
    if (!camera) return EDS_ERR_DEVICE_NOT_FOUND;
    return EdsCreateMemoryStream(0, stream);
    // Then EdsDownloadEvfImage(camera, *stream);
    // But for now, placeholder
}