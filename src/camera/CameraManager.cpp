#include "camera/CameraManager.h"

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <iostream>
#include <thread>

CameraManager::CameraManager()
    : camera(NULL), isInitialized(false), captureDirectory("") {
  // Load camera state from cache (persistent between runs)
  loadCameraState();
}

// Define static member
int CameraManager::globalSequenceNumber = 1;

CameraManager::~CameraManager() {
  // Stop live view if it's running
  stopLiveView();

  // Close camera session if open
  if (camera) {
    // Clear object event handler before closing session (like sample code)
    EdsError err =
        EdsSetObjectEventHandler(camera, kEdsObjectEvent_All, NULL, NULL);
    if (err != EDS_ERR_OK) {
      std::cout << "Failed to clear object event handler: " << err << std::endl;
    }

    err = EdsCloseSession(camera);
    if (err != EDS_ERR_OK) {
      std::cout << "Failed to close camera session: " << err << std::endl;
    } else {
      std::cout << "Camera session closed successfully" << std::endl;
    }

    // Release camera reference
    EdsRelease(camera);
    camera = NULL;
  }

  // Terminate SDK if it was initialized
  if (isInitialized) {
    EdsError err = EdsTerminateSDK();
    if (err != EDS_ERR_OK) {
      std::cout << "Failed to terminate SDK: " << err << std::endl;
    } else {
      std::cout << "EDSDK terminated successfully" << std::endl;
    }
    isInitialized = false;
  }

  // Save camera state for next run
  saveCameraState();
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

  // Set object event handler for downloading images (catch all events to ensure
  // we get transfer requests)
  err = EdsSetObjectEventHandler(camera, kEdsObjectEvent_All,
                                 objectEventHandler, this);
  if (err != EDS_ERR_OK) {
    std::cout << "Failed to set object event handler: " << err << std::endl;
    // Not critical, continue
  }

  // Set save to host initially (like sample code)
  EdsUInt32 saveTo = kEdsSaveTo_Host;
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

  // Build full path to ~/Pictures/<directory>
  const char* home = std::getenv("HOME");
  if (!home) {
    std::cout << "Could not get HOME directory" << std::endl;
    return false;
  }

  std::string picturesPath = std::string(home) + "/Pictures";
  captureDirectory = picturesPath + "/" + directory;

  // Ensure directories exist
  if (!std::filesystem::exists(picturesPath)) {
    std::filesystem::create_directories(picturesPath);
  }
  if (!std::filesystem::exists(captureDirectory)) {
    std::filesystem::create_directories(captureDirectory);
  }

  // Add a small delay to prevent rapid capture attempts
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  // Set capacity for host transfers (important when saving to host)
  setCapacityForHost();

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
    if (err == 8217) {
      std::cout << "Camera appears busy (8217). Please wait a moment before "
                   "next capture."
                << std::endl;
      return false;
    }
    return false;
  }

  // Use single command approach like sample code (without AF to avoid
  // complexity)
  err = EdsSendCommand(camera, kEdsCameraCommand_PressShutterButton,
                       kEdsCameraCommand_ShutterButton_Completely_NonAF);

  // Always release shutter button (like sample code, critical for proper camera
  // state)
  EdsError releaseErr =
      EdsSendCommand(camera, kEdsCameraCommand_PressShutterButton,
                     kEdsCameraCommand_ShutterButton_OFF);

  if (err != EDS_ERR_OK) {
    std::cout << "Capture command failed: " << err << std::endl;
    if (err == EDS_ERR_DEVICE_BUSY) {
      std::cout << "Device is busy, please wait and try again" << std::endl;
    }
  } else {
    std::cout << "Capture command sent successfully" << std::endl;
    if (releaseErr != EDS_ERR_OK) {
      std::cout << "Shutter release failed: " << releaseErr << std::endl;
    }
  }

  // Unlock UI (always attempt this, critical for recovery)
  EdsError unlockErr =
      EdsSendStatusCommand(camera, kEdsCameraStatusCommand_UIUnLock, 0);
  if (unlockErr != EDS_ERR_OK) {
    std::cout << "Failed to unlock UI: " << unlockErr << std::endl;
  } else {
    std::cout << "UI unlocked successfully" << std::endl;
  }

  // Restart live view (important for consistent camera state)
  if (!startLiveView()) {
    std::cout << "Failed to restart live view" << std::endl;
  } else {
    std::cout << "Live view restarted successfully" << std::endl;
  }

  std::cout << "Capture initiated, waiting for download to " << captureDirectory
            << "..." << std::endl;

  // Wait a moment for download to complete (max 5 seconds)
  int waitCount = 0;
  while (waitCount < 50) {  // 50 * 100ms = 5 seconds
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // Process events to check for download completion
    EdsGetEvent();
    waitCount++;
  }

  return true;
}

void CameraManager::disconnectCamera() {
  // Stop live view if running
  stopLiveView();

  if (camera) {
    // Clear object event handler
    EdsSetObjectEventHandler(camera, kEdsObjectEvent_All, NULL, NULL);

    // Close session
    EdsError err = EdsCloseSession(camera);
    if (err != EDS_ERR_OK) {
      std::cout << "Failed to close session during disconnect: " << err
                << std::endl;
    }

    // Release camera
    EdsRelease(camera);
    camera = NULL;

    std::cout << "Camera disconnected successfully" << std::endl;
  }
}

EdsError EDSCALLBACK CameraManager::objectEventHandler(EdsObjectEvent event,
                                                       EdsBaseRef object,
                                                       EdsVoid* context) {
  CameraManager* self = static_cast<CameraManager*>(context);

  switch (event) {
    case kEdsObjectEvent_DirItemRequestTransfer:
      std::cout << "DirItemRequestTransfer event received" << std::endl;
      self->downloadImage(object);
      break;
    case kEdsObjectEvent_DirItemCreated:
      std::cout << "DirItemCreated event received" << std::endl;
      self->downloadImage(object);
      break;
    default:
      std::cout << "Other object event: " << event << std::endl;
      // Don't release object for unknown events
      return EDS_ERR_OK;
  }

  // Note: object is released in downloadImage method
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

  // Generate unique filename to prevent overwriting
  std::string originalFilename = dirItemInfo.szFileName;
  std::string filename = generateUniqueFilename(originalFilename);
  std::string filepath = captureDirectory + "/" + filename;

  std::cout << "Downloading " << filename << " (" << dirItemInfo.size
            << " bytes)" << std::endl;

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
    std::cout << "Successfully downloaded to " << filepath << std::endl;
  }

  // Mark download as complete
  EdsDownloadComplete(object);

  // Release resources
  EdsRelease(stream);
  EdsRelease(object);
}

void CameraManager::setCapacityForHost() {
  // Set capacity for host transfers (important when saving to host or both)
  EdsCapacity capacity = {0x7FFFFFFF, 0x1000,
                          1};  // Max capacity, 4KB free space, 1 shot
  EdsError err = EdsSetCapacity(camera, capacity);
  if (err != EDS_ERR_OK) {
    std::cout << "Failed to set capacity: " << err << std::endl;
  } else {
    std::cout << "Capacity set for host transfers" << std::endl;
  }
}

std::string CameraManager::generateUniqueFilename(
    const std::string& originalFilename) {
  // Try to use camera filename first (already has sequence number)
  std::string filepath = captureDirectory + "/" + originalFilename;
  // If file doesn't exist, use original
  if (!std::filesystem::exists(filepath)) {
    std::cout << "Using camera filename: " << originalFilename << std::endl;
    return originalFilename;
  }

  // If file exists, add sequence number
  std::string extension =
      originalFilename.substr(originalFilename.find_last_of('.'));
  std::string basename =
      originalFilename.substr(0, originalFilename.find_last_of('.'));

  int sequence = cameraState.sequenceNumber;
  std::string newFilename;
  do {
    newFilename = basename + "_" + std::to_string(sequence) + extension;
    filepath = captureDirectory + "/" + newFilename;
    sequence++;
  } while (std::filesystem::exists(filepath));

  // Update global sequence for next time
  cameraState.sequenceNumber = sequence;
  std::cout << "File exists, using: " << newFilename << std::endl;
  return newFilename;
}

void CameraManager::loadCameraState() {
  const char* home = std::getenv("HOME");
  if (!home) return;

  std::string cacheDir = std::string(home) + "/.cache/cocoscanner";
  std::filesystem::create_directories(cacheDir);

  std::string stateFile = cacheDir + "/camera_state.json";
  std::ifstream file(stateFile);

  if (file.is_open()) {
    // Simple JSON parsing for our state
    std::string line;
    while (std::getline(file, line)) {
      if (line.find("\"sequenceNumber\"") != std::string::npos) {
        size_t pos = line.find(":") + 1;
        if (pos != std::string::npos) {
          cameraState.sequenceNumber = std::stoi(line.substr(pos));
        }
      } else if (line.find("\"lastCassette\"") != std::string::npos) {
        size_t pos = line.find(":") + 1;
        if (pos != std::string::npos) {
          cameraState.lastCassette = line.substr(pos + 1);
          // Remove quotes
          if (cameraState.lastCassette.length() >= 2) {
            cameraState.lastCassette = cameraState.lastCassette.substr(
                1, cameraState.lastCassette.length() - 2);
          }
        }
      } else if (line.find("\"lastDate\"") != std::string::npos) {
        size_t pos = line.find(":") + 1;
        if (pos != std::string::npos) {
          cameraState.lastDate = line.substr(pos + 1);
          // Remove quotes
          if (cameraState.lastDate.length() >= 2) {
            cameraState.lastDate = cameraState.lastDate.substr(
                1, cameraState.lastDate.length() - 2);
          }
        }
      }
    }
    file.close();
    std::cout << "Loaded camera state, sequence: " << cameraState.sequenceNumber
              << std::endl;
  } else {
    std::cout << "No camera state file found, using defaults" << std::endl;
  }
}

void CameraManager::saveCameraState() {
  const char* home = std::getenv("HOME");
  if (!home) return;

  std::string cacheDir = std::string(home) + "/.cache/cocoscanner";
  std::filesystem::create_directories(cacheDir);

  std::string stateFile = cacheDir + "/camera_state.json";
  std::ofstream file(stateFile);

  if (file.is_open()) {
    file << "{\n";
    file << "  \"sequenceNumber\": " << cameraState.sequenceNumber << ",\n";
    file << "  \"lastCassette\": \"" << cameraState.lastCassette << "\",\n";
    file << "  \"lastDate\": \"" << cameraState.lastDate << "\"\n";
    file << "}\n";
    file.close();
    std::cout << "Saved camera state" << std::endl;
  }
}

void CameraManager::resetCameraState() {
  cameraState.sequenceNumber = 1;
  cameraState.lastCassette = "";
  cameraState.lastDate = "";
  saveCameraState();
  std::cout << "Reset camera state to defaults" << std::endl;
}
