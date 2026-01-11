#include "CameraManager.h"
#include <iostream>
#include <cstring>

CameraManager::CameraManager() 
    : m_camera(nullptr)
    , m_cameraList(nullptr)
    , m_initialized(false)
    , m_connected(false) {
}

CameraManager::~CameraManager() {
    disconnect();
    if (m_initialized) {
        EdsTerminateSDK();
    }
}

bool CameraManager::initialize() {
    EdsError err = EdsInitializeSDK();
    if (err != EDS_ERR_OK) {
        std::cerr << "Failed to initialize EDSDK: " << err << std::endl;
        return false;
    }
    
    m_initialized = true;
    return true;
}

bool CameraManager::connectToCamera() {
    if (!m_initialized) {
        return false;
    }
    
    // Get camera list
    EdsError err = EdsGetCameraList(&m_cameraList);
    if (err != EDS_ERR_OK) {
        std::cerr << "Failed to get camera list: " << err << std::endl;
        return false;
    }
    
    // Get number of cameras
    EdsUInt32 count = 0;
    err = EdsGetChildCount(m_cameraList, &count);
    if (err != EDS_ERR_OK || count == 0) {
        std::cerr << "No cameras found" << std::endl;
        return false;
    }
    
    // Get first camera
    err = EdsGetChildAtIndex(m_cameraList, 0, &m_camera);
    if (err != EDS_ERR_OK) {
        std::cerr << "Failed to get camera: " << err << std::endl;
        return false;
    }
    
    // Set event handlers
    EdsSetObjectEventHandler(m_camera, kEdsObjectEvent_All, handleObjectEvent, this);
    EdsSetPropertyEventHandler(m_camera, kEdsPropertyEvent_All, handlePropertyEvent, this);
    EdsSetCameraStateEventHandler(m_camera, kEdsStateEvent_All, handleStateEvent, this);
    
    // Open session
    err = EdsOpenSession(m_camera);
    if (err != EDS_ERR_OK) {
        std::cerr << "Failed to open camera session: " << err << std::endl;
        return false;
    }
    
    m_connected = true;
    std::cout << "Connected to camera successfully" << std::endl;
    return true;
}

void CameraManager::disconnect() {
    if (m_connected && m_camera) {
        EdsCloseSession(m_camera);
        m_connected = false;
    }
    
    if (m_camera) {
        EdsRelease(m_camera);
        m_camera = nullptr;
    }
    
    if (m_cameraList) {
        EdsRelease(m_cameraList);
        m_cameraList = nullptr;
    }
}

bool CameraManager::isConnected() const {
    return m_connected;
}

EdsError EDSCALLBACK CameraManager::handleObjectEvent(EdsObjectEvent event, EdsBaseRef object, EdsVoid* context) {
    // Handle object events (like new files)
    if (object) {
        EdsRelease(object);
    }
    return EDS_ERR_OK;
}

EdsError EDSCALLBACK CameraManager::handlePropertyEvent(EdsPropertyEvent event, EdsPropertyID property, EdsUInt32 data, EdsVoid* context) {
    // Handle property changes
    return EDS_ERR_OK;
}

EdsError EDSCALLBACK CameraManager::handleStateEvent(EdsStateEvent event, EdsUInt32 data, EdsVoid* context) {
    // Handle camera state changes
    return EDS_ERR_OK;
}