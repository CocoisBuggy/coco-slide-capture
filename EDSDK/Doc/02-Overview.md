# Overview

## 2.1 Protocol for Remote Connection

PTP is an abbreviation of "Picture Transfer Protocol." PTP is a standard protocol used to transfer images to a PC. A device driver for each model is unnecessary when connecting to an OS that supports PTP.

## 2.2 System Architecture

The following figure shows the configuration of software when a Canon digital camera has been connected.

```
Windows                 Your Application
                         |
EDSDK API IF            |              Macintosh              Your Application
                         |
Canon                    |                    Canon              EDSDK.framework
Library Modules          |                    Library Modules
                         |
Microsoft                | WIA / WPD           Apple                     ICA
                         |
Microsoft                | PTP driver          Apple                 PTP driver
                         |
Kernel Mode              | Driver for USB      Kernel Mode               Driver for USB
                         |
Canon digital camera      |                    Canon digital camera
```

**Note:** Use the OS standard driver for the EOS digital driver when using a camera that uses PTP for the remote connection protocol when connecting to an OS that supports PTP. Otherwise, the driver provided by Canon must be used.

## 2.3 Library Modules

The following figure shows the module configuration of EDSDK.

**Windows:**
- EDSDK.dll
- EdsImage.dll

**Macintosh:**
- EDSDK.framework
- EdsImage.bundle

**Linux:**
- libEDSDK.so

## 2.4 EDSDK Objects

As shown in the hierarchical structure, EDSDK employs a hierarchical structure with a camera list at the root in order to control and access cameras connected to the host PC. This hierarchical structure consists of the following elements: camera list, cameras, volumes, folders, image files, audio files, etc.

These elements are treated as belonging to one of the following object categories: EdsCameraListRef, EdsCameraRef, EdsVolumeRef, and EdsDirectoryItemRef. Having a hierarchical structure, these four objects may have child objects.

### Object Hierarchy

```
CameraList (EdsCameraListRef)
├── Camera #1 (EdsCameraRef)
│   ├── Volume #1 (EdsVolumeRef)
│   │   ├── Folder #1 (EdsDirectoryItemRef)
│   │   └── Folder #2 (EdsDirectoryItemRef)
│   └── Volume #2 (EdsVolumeRef)
│       └── Folder #3 (EdsDirectoryItemRef)
└── Camera #2 (EdsCameraRef)
```

### Object Types

1. **EdsCameraListRef**
   - Represents an enumeration of cameras remotely connected to the host PC by USB interface
   - Used to select the camera to be controlled from among connected cameras
   - Used to get an EdsCameraRef child object

2. **EdsCameraRef**
   - Represents a remotely connected camera
   - Used to control the camera or get an EdsVolumeRef object when accessing the memory card

3. **EdsVolumeRef**
   - Represents the memory card inside the camera
   - If the camera model allows two memory cards, each memory card has its own EdsVolumeRef object
   - Used to get an EdsDirectoryItemRef object for file/folder operations

4. **EdsDirectoryItemRef**
   - Represents a file or folder on the camera
   - When files are downloaded from the camera, each file is treated as one of these objects

5. **EdsImageRef**
   - Represents image data obtained from image files
   - Used to retrieve and control information included with an image such as thumbnails and parameters

6. **EdsStreamRef**
   - Represents the file I/O stream
   - Can be specified as the download destination when downloading files
   - Can be created in memory or from files on the host PC

7. **EdsEvfImageRef**
   - Represents PC live view image data
   - When using a camera model that supports live view, live view image data can be downloaded
   - Includes information such as zoom and histogram data

## 2.5 Object Management

### 2.5.1 Object Management Using a Reference Counter

Applications built using the EDSDK carry out object management using a reference counter. EDSDK stores a reference counter for all objects. The reference counter is set to 1 when an object has been allocated.

The developer increases the reference counter by 1 when the object is required by the program, and lowers it by 1 when the object is no longer needed. When a reference counter reaches 0, the associated object is automatically deleted by the EDSDK.

EdsRetain and EdsRelease are provided as APIs for controlling object reference counters.

### 2.5.2 Releasing Resources when Exiting the Library

Applications built using the EDSDK will release all allocated resources when EdsTerminateSDK is called.

## 2.6 Properties

Properties are stored under EDSDK for camera and image objects. For example, properties may represent values such as camera Av and Tv. The functions EdsGetPropertyData and EdsSetPropertyData are used to get and set these properties.

Since this API takes objects of undefined type as arguments, the properties that can be retrieved or set differ depending on the given object. Some properties have a list of currently settable values. EdsGetPropertyDesc is used to get this list of settable values.

## 2.7 Camera Status

Cameras remotely connected to the host PC can be in one of several states:

### (1) UI Lock
- All operations of the camera unit are disabled and only operations from the host PC are accepted
- Allows data and instructions to be safely sent from host PC to the camera

### (2) UI Lock Release
- Operations of the camera unit are enabled
- Data and instructions can be sent from host PC to the camera, but conflicts may arise

### (3) Direct Transfer (for models with an Easy Direct button)
- The camera is currently directly transferring data
- Available camera operations are limited to functions related to direct transfer
- A direct transfer request event notification is issued to the EDSDK client application when an operation for starting image download is initiated using camera controls

### (4) Direct Transfer Release
- Direct transfer is not currently being carried out

## 2.8 Asynchronous Events

An asynchronous event is a mechanism used to issue notifications from the EDSDK to the application regarding cameras connected to the host PC or state changes that have occurred for a camera.

An event handler capable of specific processing required for a particular event must be registered in order to receive such an event. An event handler is a user function called when an event is received. Event handlers are also referred to as "callback functions."

There are three types of events issued from EDSDK to a client application:

### (1) Object-related events
- Request notifications to create, delete, or transfer image data stored in a remotely connected camera or image files on the memory card

### (2) Property-related events
- Notifications regarding changes in the properties of a remotely connected camera

### (3) State-related events
- Notifications regarding changes in the state of a remotely connected camera, such as activation of a shut-down timer

## 2.9 Initializing and Terminating the Library

The user must initialize the EDSDK library in order to use EDSDK functions other than those for getting device information from a camera. The user must also terminate the library when EDSDK functions are no longer needed.

Be sure to execute initialization and termination of the library once each within the application process.

## 2.10 Accessing a Camera

The EDSDK provides methods of accessing and controlling a camera. To allow access to more than one camera connected to the host PC, it is possible to get all camera objects by repeatedly calling EdsGetChildAtIndex by specifying an index of child objects on the camera list.

The number of cameras connected can be obtained using EdsGetChildCount. Specify 0 as the index passed to EdsGetChildAtIndex if there is only one camera.

An EDSDK client application can open a session with any one of the connected cameras. Opening a session means connecting to a camera at the application level so that it is possible to control that camera from the application and get associated properties and events. To open a session, specify the camera in question and call EdsOpenSession. Open sessions must be closed using EdsCloseSession when communications are finished.

### Notes on Developing Windows Applications

When creating applications that run under Windows, a COM initialization is required for each thread in order to access a camera from a thread other than the main thread.

To create a user thread and access a camera from that thread, be sure to execute `CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)` at the start of the thread and `CoUninitialize()` at the end.

### Notes on Developing Macintosh Applications

On macOS 13 (Ventura) or later, the number of cameras may be returned as 0 when getting the camera list. In such cases, try runloop processing before getting the camera list.

## 2.11 Transferring Files in the Camera

This section describes how to access files in the camera and transfer them to the host PC. Although it is possible to access the camera and control properties of files (such as date of creation and protection settings), it is not possible to analyze file properties. Files must therefore be transferred in order to get file properties. A method for transferring thumbnails (header information) only is also provided for such cases.

## 2.12 Transferring Captured Images

When a shoot command is sent from the host PC to the camera, the camera will record the image shot in a buffer inside the camera. Once the shot has been taken, callback functions set using EdsSetPropertyEventHandler, EdsSetObjectEventHandler, and EdsSetCameraStateEventHandler will be called by the EDSDK. The user must sequentially transfer images stored in the camera buffer to the host PC.

## 2.13 Handling Image Objects

### 2.13.1 Overview

As touched on in the section on EDSDK objects, it is impossible to get an image object reference from an image file stored in a camera. An image object reference can only be obtained after first downloading the image file to a host PC.

An image object is an object that has properties. Camera properties such as Tv and Av that are used while shooting images are stored and can be obtained using EdsGetPropertyData.

### 2.13.2 Getting and Setting Properties

The sequence for getting properties from a camera image involves:
1. Getting directory item information
2. Creating a transfer destination stream
3. Downloading the image
4. Creating an image reference from the stream
5. Getting properties from the image object
6. Releasing the image object and stream

## 2.14 Basic Data Type Definitions

This section introduces the basic data types used under the EDSDK. These data types are defined as C language types:

```c
typedef void                       EdsVoid;
typedef int                        EdsBool;

typedef char                       EdsChar;
typedef char                       EdsInt8;
typedef unsigned char              EdsUInt8;
typedef short                      EdsInt16;
typedef unsigned short             EdsUInt16;
typedef long                       EdsInt32;
typedef unsigned long              EdsUInt32;

#ifdef __MACOS__
#ifdef __cplusplus
     typedef long long             EdsInt64;
     typedef unsigned long long    EdsUInt64;
#else
     typedef SInt64                EdsInt64;
     typedef UInt64                EdsUInt64;
#endif
#else
     typedef __int64               EdsInt64;
     typedef unsigned __int64      EdsUInt64;
#endif

typedef float                      EdsFloat;
typedef double                     EdsDouble;
```

## 2.15 EDSDK Errors

Most of the APIs supplied by EDSDK return an error code of type EdsError as their return value.

The return value of an API that terminates normally is EDS_ERR_OK. If an error occurs, the return value of the API is set to the error code indicating the root cause of the error and any passed parameters are stored as undefined values.

For error codes, see the list given in the header file EdsError.h or see EDS Error Lists at the end of the section describing APIs in this document.

---

**Next:** [API Reference](03-API-Reference.md) | **Previous:** [Introduction](01-Introduction.md)