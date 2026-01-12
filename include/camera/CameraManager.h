#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <EDSDK.h>

#include <filesystem>
#include <fstream>
#include <string>

// Forward declaration
class LiveViewRenderer;

// Persistent camera state data structure
struct CameraState {
  int sequenceNumber = 1;
  std::string lastCassette;
  std::string lastDate;
  // Add other camera state as needed
};

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
  bool capture(const std::string& directory, const std::string& comment = "",
               LiveViewRenderer* renderer = nullptr);
  void disconnectCamera();

 private:
  EdsCameraRef camera;
  bool isInitialized;
  std::string captureDirectory;

  // Persistent camera state
  CameraState cameraState;

  // Current photo comment
  std::string currentComment;

  // Global sequence number for unique filenames
  static int globalSequenceNumber;

  static EdsError EDSCALLBACK objectEventHandler(EdsObjectEvent event,
                                                 EdsBaseRef object,
                                                 EdsVoid* context);
  void downloadImage(EdsBaseRef object);
  void setCapacityForHost();
  std::string generateUniqueFilename(const std::string& originalFilename);

  // Persistent state management
  void loadCameraState();
  void saveCameraState();
  void resetCameraState();

  // State setters with auto-save
  void setCassette(const std::string& cassette);
  void setCurrentDate();
};

#endif  // CAMERAMANAGER_H