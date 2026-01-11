#include "camera/CameraManager.h"

#include <chrono>
#include <ctime>
#include <filesystem>
#include <iostream>
#include <thread>

CameraManager::CameraManager()
    : camera(NULL), isInitialized(false), captureDirectory("") {}

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

  // Set object event handler for downloading images
  err = EdsSetObjectEventHandler(camera, kEdsObjectEvent_DirItemCreated,
                                 objectEventHandler, this);
  if (err != EDS_ERR_OK) {
    std::cout << "Failed to set object event handler: " << err << std::endl;
    // Not critical, continue
  }

  // Set save to both (card and host)
  EdsUInt32 saveTo = kEdsSaveTo_Both;
  err = EdsSetPropertyData(camera, kEdsPropID_SaveTo, 0, sizeof(EdsUInt32),
                           &saveTo);
  if (err != EDS_ERR_OK) {
    std::cout << "Failed to set save to both: " << err << std::endl;
    // Not critical, continue
  }

  // Set AE mode to Manual to allow capture
  EdsUInt32 aeMode = kEdsAEMode_Manual;
  err = EdsSetPropertyData(camera, kEdsPropID_AEMode, 0, sizeof(EdsUInt32),
                           &aeMode);
  if (err != EDS_ERR_OK) {
    std::cout << "Failed to set AE mode to Manual: " << err << std::endl;
    // Not critical, continue
  }

  std::cout << "Camera connected" << std::endl;
  return true;
}

bool CameraManager::startLiveView() {
  if (!camera) return false;
  EdsUInt32 mode = 1;  // Enable live view mode
  EdsError err = EdsSetPropertyData(camera, kEdsPropID_Evf_Mode, 0,
                                    sizeof(EdsUInt32), &mode);
  if (err != EDS_ERR_OK) {
    std::cout << "Failed to set live view mode: " << err << std::endl;
    return false;
  }
  EdsUInt32 device = kEdsEvfOutputDevice_PC | kEdsEvfOutputDevice_TFT;
  err = EdsSetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0,
                           sizeof(EdsUInt32), &device);
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
  EdsError err = EdsSetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0,
                                    sizeof(EdsUInt32), &device);
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

bool CameraManager::capture(const std::string& directory) {
  if (!camera) return false;
  captureDirectory = directory;

  // Ensure directory exists
  if (!std::filesystem::exists(directory)) {
    std::filesystem::create_directories(directory);
  }

  // Check save destination
  EdsUInt32 saveTo;
  EdsError err = EdsGetPropertyData(camera, kEdsPropID_SaveTo, 0,
                                    sizeof(EdsUInt32), &saveTo);
  if (err != EDS_ERR_OK) {
    std::cout << "Failed to get SaveTo: " << err << std::endl;
  } else {
    std::cout << "SaveTo: " << saveTo << " (should be " << kEdsSaveTo_Both
              << ")" << std::endl;
  }

  // Lock UI for capture
  err = EdsSendStatusCommand(camera, kEdsCameraStatusCommand_UILock, 0);
  if (err != EDS_ERR_OK) {
    std::cout << "Failed to lock UI: " << err << std::endl;
    return false;
  }

  // Enter direct transfer mode for host save
  err = EdsSendStatusCommand(camera,
                             kEdsCameraStatusCommand_EnterDirectTransfer, 0);
  if (err != EDS_ERR_OK) {
    std::cout << "Release shutter failed: " << err << std::endl;
    EdsSendStatusCommand(camera, kEdsCameraStatusCommand_ExitDirectTransfer, 0);
    EdsSendStatusCommand(camera, kEdsCameraStatusCommand_UIUnLock, 0);
    return false;
  }

  // Enter direct transfer mode
  err = EdsSendStatusCommand(camera,
                             kEdsCameraStatusCommand_EnterDirectTransfer, 0);
  if (err != EDS_ERR_OK) {
    std::cout << "Failed to enter direct transfer: " << err << std::endl;
    return false;
  }

  // Half press for auto focus and auto expose
  err = EdsSendCommand(camera, kEdsCameraCommand_PressShutterButton,
                       kEdsCameraCommand_ShutterButton_Halfway);
  if (err != EDS_ERR_OK) {
    std::cout << "Half press failed: " << err << std::endl;
    return false;
  }

  // Wait a bit for AF/AE
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  // Full press to take photo
  err = EdsSendCommand(camera, kEdsCameraCommand_PressShutterButton,
                       kEdsCameraCommand_ShutterButton_Completely);
  if (err != EDS_ERR_OK) {
    std::cout << "Full press failed: " << err << std::endl;
    return false;
  }

  // Release shutter
  err = EdsSendCommand(camera, kEdsCameraCommand_PressShutterButton,
                       kEdsCameraCommand_ShutterButton_OFF);
  if (err != EDS_ERR_OK) {
    std::cout << "Release shutter failed: " << err << std::endl;
    startLiveView();  // Try to restart live view
    return false;
  }

  // Exit direct transfer mode
  err = EdsSendStatusCommand(camera, kEdsCameraStatusCommand_ExitDirectTransfer,
                             0);
  if (err != EDS_ERR_OK) {
    std::cout << "Failed to exit direct transfer: " << err << std::endl;
  }

  // Unlock UI
  err = EdsSendStatusCommand(camera, kEdsCameraStatusCommand_UIUnLock, 0);
  if (err != EDS_ERR_OK) {
    std::cout << "Failed to unlock UI: " << err << std::endl;
  }

  // Restart live view
  if (!startLiveView()) {
    std::cout << "Failed to restart live view" << std::endl;
  }

  std::cout << "Capture initiated" << std::endl;
  return true;
}

EdsError EDSCALLBACK CameraManager::objectEventHandler(EdsObjectEvent event,
                                                       EdsBaseRef object,
                                                       EdsVoid* context) {
  if (event == kEdsObjectEvent_DirItemCreated) {
    CameraManager* self = static_cast<CameraManager*>(context);
    self->downloadImage(object);
  }
  return EDS_ERR_OK;
}

void CameraManager::downloadImage(EdsBaseRef object) {
  EdsDirectoryItemInfo dirItemInfo;
  EdsError err = EdsGetDirectoryItemInfo(object, &dirItemInfo);
  if (err != EDS_ERR_OK) {
    std::cout << "Failed to get directory item info: " << err << std::endl;
    EdsRelease(object);
    return;
  }

  // Generate filename with timestamp
  std::time_t now = std::time(nullptr);
  std::string filename = std::to_string(now) + ".CR2";
  std::string filepath = captureDirectory + "/" + filename;

  // Create file stream
  EdsStreamRef stream;
  err = EdsCreateFileStream(filepath.c_str(),
                            kEdsFileCreateDisposition_CreateAlways,
                            kEdsAccess_ReadWrite, &stream);
  if (err != EDS_ERR_OK) {
    std::cout << "Failed to create file stream: " << err << std::endl;
    EdsRelease(object);
    return;
  }

  // Download
  err = EdsDownload(object, dirItemInfo.size, stream);
  if (err != EDS_ERR_OK) {
    std::cout << "Failed to download: " << err << std::endl;
  } else {
    std::cout << "Downloaded to " << filepath << std::endl;
  }

  EdsDownloadComplete(object);
  EdsRelease(stream);
  EdsRelease(object);
}