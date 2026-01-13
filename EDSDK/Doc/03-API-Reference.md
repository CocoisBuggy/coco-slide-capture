# API Reference

## 3.1 API Details

API specifications are explained in the following format:

### Description
Indicates the main API function.

### Syntax
```c
EdsError EdsXXXXX( EdsUInt32 inXXXX, EdsBaseRef *outXXX );
```
Indicates the syntax for calling the API.

### Parameters
Explains each argument in the syntax individually.
- Argument names in format `inXXX` represent arguments for which you enter values.
- Argument names in format `outXXX` represent arguments with values set by libraries (passed by reference). Before calling APIs, you must prepare variables for storing the data to be retrieved.

### Return Values
Explains API return values.

### See Also
Indicates information related to the API.

### Note
Considerations when using the API.

### Example
Sample code.

---

## SDK Initialization & Management

### 3.1.1 EdsInitializeSDK
**Description**
Initializes the libraries. When using the EDSDK libraries, you must call this API once before using EDSDK APIs.

**Syntax**
```c
EdsError EdsInitializeSDK()
```

**Parameters**
None

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

**See Also**
- Related APIs: EdsTerminateSDK

**Example**
- See Sample 1.

---

### 3.1.2 EdsTerminateSDK
**Description**
Terminates use of the libraries. Calling this function releases all resources allocated by the libraries.

**Syntax**
```c
EdsError EdsTerminateSDK()
```

**Parameters**
None

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

**See Also**
- Related APIs: EdsInitializeSDK

**Example**
- See Sample 1.

---

### 3.1.3 EdsRetain
**Description**
Increments the reference counter of existing objects.

**Syntax**
```c
EdsError EdsRetain(EdsBaseRef inRef)
```

**Parameters**
- `inRef`: Object for which to increment the reference counter

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.4 EdsRelease
**Description**
Decrements the reference counter to an object. When the reference counter reaches 0, the object is released.

**Syntax**
```c
EdsError EdsRelease(EdsBaseRef inRef)
```

**Parameters**
- `inRef`: Object for which to decrement the reference counter

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

## Object Navigation

### 3.1.5 EdsGetChildCount
**Description**
Gets the number of child objects of the designated object. Example: Number of files in a directory.

**Syntax**
```c
EdsError EdsGetChildCount(EdsBaseRef inRef, EdsUInt32 *outCount)
```

**Parameters**
- `inRef`: Object whose child objects are to be counted
- `outCount`: Number of child objects

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.6 EdsGetChildAtIndex
**Description**
Gets an indexed child object of the designated object.

**Syntax**
```c
EdsError EdsGetChildAtIndex(EdsBaseRef inRef, EdsUInt32 inIndex, EdsBaseRef *outChildRef)
```

**Parameters**
- `inRef`: Object whose child object is to be retrieved
- `inIndex`: Index of child object to retrieve
- `outChildRef`: Child object retrieved

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.7 EdsGetParent
**Description**
Gets the parent object of the designated object.

**Syntax**
```c
EdsError EdsGetParent(EdsBaseRef inRef, EdsBaseRef *outParentRef)
```

**Parameters**
- `inRef`: Object whose parent is to be retrieved
- `outParentRef`: Parent object retrieved

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.8 EdsGetCameraList
**Description**
Gets camera list objects.

**Syntax**
```c
EdsError EdsGetCameraList(EdsCameraListRef *outCameraListRef)
```

**Parameters**
- `outCameraListRef`: Camera list object

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

## Camera & Device Information

### 3.1.9 EdsGetDeviceInfo
**Description**
Gets device information, such as the device name. Because device information of remote cameras is stored on the host computer, you can use this API before the camera object initiates communication (before a session is opened).

**Syntax**
```c
EdsError EdsGetDeviceInfo(EdsCameraRef inCamera, EdsDeviceInfo *outDeviceInfo)
```

**Parameters**
- `inCamera`: Camera object
- `outDeviceInfo`: Device information structure

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.10 EdsGetVolumeInfo
**Description**
Gets volume information for a memory card in the camera.

**Syntax**
```c
EdsError EdsGetVolumeInfo(EdsVolumeRef inVolume, EdsVolumeInfo *outVolumeInfo)
```

**Parameters**
- `inVolume`: Volume object
- `outVolumeInfo`: Volume information structure

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.11 EdsGetDirectoryItemInfo
**Description**
Gets information about directory or file objects on the memory card (volume) in a remote camera.

**Syntax**
```c
EdsError EdsGetDirectoryItemInfo(EdsDirectoryItemRef inDirItem, EdsDirectoryItemInfo *outDirItemInfo)
```

**Parameters**
- `inDirItem`: Directory item object
- `outDirItemInfo`: Directory item information structure

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

## Camera Sessions

### 3.1.12 EdsOpenSession
**Description**
Establishes a logical connection with a remote camera. Use this API after getting the camera's EdsCamera object.

**Syntax**
```c
EdsError EdsOpenSession(EdsCameraRef inCamera)
```

**Parameters**
- `inCamera`: Camera object

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.13 EdsCloseSession
**Description**
Closes a logical connection with a remote camera.

**Syntax**
```c
EdsError EdsCloseSession(EdsCameraRef inCamera)
```

**Parameters**
- `inCamera`: Camera object

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

## Camera Commands

### 3.1.14 EdsSendCommand
**Description**
Sends a command such as "Shoot" to a remote camera.

**Syntax**
```c
EdsError EdsSendCommand(EdsCameraRef inCamera, EdsUInt32 inCommand, EdsInt32 inParam)
```

**Parameters**
- `inCamera`: Camera object
- `inCommand`: Command to send
- `inParam`: Command parameter

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.15 EdsSendStatusCommand
**Description**
Sets the remote camera state or mode.

**Syntax**
```c
EdsError EdsSendStatusCommand(EdsCameraRef inCamera, EdsUInt32 inStatusCommand)
```

**Parameters**
- `inCamera`: Camera object
- `inStatusCommand`: Status command to send

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.16 EdsSetCapacity
**Description**
Sets the remaining HDD capacity on the host computer (excluding portion from image transfer), as calculated by subtracting portion from previous time.

**Syntax**
```c
EdsError EdsSetCapacity(EdsCameraRef inCamera, EdsUInt32 inCapacity)
```

**Parameters**
- `inCamera`: Camera object
- `inCapacity`: Capacity to set

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

## Property Management

### 3.1.17 EdsGetPropertySize
**Description**
Gets the byte size and data type of a designated property from a camera object or image object.

**Syntax**
```c
EdsError EdsGetPropertySize(EdsBaseRef inRef, EdsUInt32 inPropertyID, EdsUInt32 inParam, EdsUInt32 *outSize, EdsDataType *outDataType)
```

**Parameters**
- `inRef`: Object
- `inPropertyID`: Property ID
- `inParam`: Property parameter
- `outSize`: Property data size
- `outDataType`: Property data type

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.18 EdsGetPropertyData
**Description**
Gets property information from the object designated in inRef.

**Syntax**
```c
EdsError EdsGetPropertyData(EdsBaseRef inRef, EdsUInt32 inPropertyID, EdsUInt32 inParam, EdsUInt32 inSize, void *outPropertyData)
```

**Parameters**
- `inRef`: Object
- `inPropertyID`: Property ID
- `inParam`: Property parameter
- `inSize`: Size of property data buffer
- `outPropertyData`: Property data buffer

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.19 EdsSetPropertyData
**Description**
Sets property data for the object designated in inRef.

**Syntax**
```c
EdsError EdsSetPropertyData(EdsBaseRef inRef, EdsUInt32 inPropertyID, EdsInt32 inParam, EdsUInt32 inSize, const void *inPropertyData)
```

**Parameters**
- `inRef`: Object
- `inPropertyID`: Property ID
- `inParam`: Property parameter
- `inSize`: Size of property data
- `inPropertyData`: Property data to set

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.20 EdsGetPropertyDesc
**Description**
Gets a list of property data that can be set for the object designated in inRef, as well as maximum and minimum values.

**Syntax**
```c
EdsError EdsGetPropertyDesc(EdsBaseRef inRef, EdsUInt32 inPropertyID, EdsPropertyDesc *outPropertyDesc)
```

**Parameters**
- `inRef`: Object
- `inPropertyID`: Property ID
- `outPropertyDesc`: Property description structure

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

## File Operations

### 3.1.21 EdsDeleteDirectoryItem
**Description**
Deletes a camera folder or file. If folders with subdirectories are designated, all files are deleted except protected files.

**Syntax**
```c
EdsError EdsDeleteDirectoryItem(EdsDirectoryItemRef inDirItem)
```

**Parameters**
- `inDirItem`: Directory item to delete

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.22 EdsFormatVolume
**Description**
Formats volumes of memory cards in a camera.

**Syntax**
```c
EdsError EdsFormatVolume(EdsVolumeRef inVolume)
```

**Parameters**
- `inVolume`: Volume to format

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.23 EdsGetAttribute
**Description**
Gets attributes of files on a camera.

**Syntax**
```c
EdsError EdsGetAttribute(EdsDirectoryItemRef inDirItem, EdsUInt32 *outAttribute)
```

**Parameters**
- `inDirItem`: Directory item
- `outAttribute`: File attributes

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.24 EdsSetAttribute
**Description**
Changes attributes of files on a camera.

**Syntax**
```c
EdsError EdsSetAttribute(EdsDirectoryItemRef inDirItem, EdsUInt32 inAttribute)
```

**Parameters**
- `inDirItem`: Directory item
- `inAttribute`: File attributes to set

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

## File Download Operations

### 3.1.25 EdsDownload
**Description**
Downloads a file on a remote camera (in the camera memory or on a memory card) to the host computer.

**Syntax**
```c
EdsError EdsDownload(EdsDirectoryItemRef inDirItem, EdsStreamRef inDestStream)
```

**Parameters**
- `inDirItem`: Directory item to download
- `inDestStream`: Destination stream

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.26 EdsDownloadComplete
**Description**
Must be called when downloading of directory items is complete. Executing this API makes the camera recognize that file transmission is complete.

**Syntax**
```c
EdsError EdsDownloadComplete(EdsDirectoryItemRef inDirItem)
```

**Parameters**
- `inDirItem`: Directory item

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.27 EdsDownloadCancel
**Description**
Must be executed when downloading of a directory item is canceled. Calling this API makes the camera cancel file transmission.

**Syntax**
```c
EdsError EdsDownloadCancel(EdsDirectoryItemRef inDirItem)
```

**Parameters**
- `inDirItem`: Directory item

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.28 EdsDownloadThumbnail
**Description**
Extracts and downloads thumbnail information from image files in a camera.

**Syntax**
```c
EdsError EdsDownloadThumbnail(EdsDirectoryItemRef inDirItem, EdsStreamRef inDestStream)
```

**Parameters**
- `inDirItem`: Directory item
- `inDestStream`: Destination stream

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

## Live View Operations

### 3.1.29 EdsCreateEvfImageRef
**Description**
Creates an object used to get the live view image data set.

**Syntax**
```c
EdsError EdsCreateEvfImageRef(EdsEvfImageRef *outEvfImageRef)
```

**Parameters**
- `outEvfImageRef`: Live view image object

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.30 EdsDownloadEvfImage
**Description**
Downloads the live view image data set for a camera currently in live view mode.

**Syntax**
```c
EdsError EdsDownloadEvfImage(EdsCameraRef inCamera, EdsEvfImageRef inEvfImageRef)
```

**Parameters**
- `inCamera`: Camera object
- `inEvfImageRef`: Live view image object

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

## Stream Management

### 3.1.31 EdsCreateFileStream
**Description**
Creates a new file on a host computer (or opens an existing file) and creates a file stream for access to the file.

**Syntax**
```c
EdsError EdsCreateFileStream(const EdsChar *inFileName, EdsFileCreateDisposition inCreateDisposition, EdsAccess inDesiredAccess, EdsStreamRef *outStream)
```

**Parameters**
- `inFileName`: File name
- `inCreateDisposition`: File creation disposition
- `inDesiredAccess`: Desired access
- `outStream`: Stream object

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.32 EdsCreateFileStreamEx
**Description**
An extended version of EdsCreateFileStream. Use this function when working with Unicode file names.

**Syntax**
```c
EdsError EdsCreateFileStreamEx(const EdsChar *inFileName, EdsFileCreateDisposition inCreateDisposition, EdsAccess inDesiredAccess, EdsStreamRef *outStream)
```

**Parameters**
- `inFileName`: File name
- `inCreateDisposition`: File creation disposition
- `inDesiredAccess`: Desired access
- `outStream`: Stream object

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.33 EdsCreateMemoryStream
**Description**
Creates a stream in the memory of a host computer.

**Syntax**
```c
EdsError EdsCreateMemoryStream(EdsUInt32 inSize, EdsStreamRef *outStream)
```

**Parameters**
- `inSize`: Stream size
- `outStream`: Stream object

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.34 EdsCreateMemoryStreamFromPointer
**Description**
Creates a stream from a memory buffer you prepare.

**Syntax**
```c
EdsError EdsCreateMemoryStreamFromPointer(void *inBuffer, EdsUInt32 inSize, EdsStreamRef *outStream)
```

**Parameters**
- `inBuffer`: Memory buffer
- `inSize`: Buffer size
- `outStream`: Stream object

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.35 EdsGetPointer
**Description**
Gets the pointer to the start address of memory managed by the memory stream.

**Syntax**
```c
EdsError EdsGetPointer(EdsStreamRef inStream, void **outPointer)
```

**Parameters**
- `inStream`: Stream object
- `outPointer`: Memory pointer

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

## Stream I/O Operations

### 3.1.36 EdsRead
**Description**
Reads data of size of inReadSize into the outBuffer buffer, starting at the current read or write position of the stream.

**Syntax**
```c
EdsError EdsRead(EdsStreamRef inStream, EdsUInt32 inReadSize, EdsUInt8 *outBuffer, EdsUInt32 *outReadSize)
```

**Parameters**
- `inStream`: Stream object
- `inReadSize`: Size to read
- `outBuffer`: Buffer to read into
- `outReadSize`: Actual size read

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.37 EdsWrite
**Description**
Writes data of a designated buffer to the current read or write position of the stream.

**Syntax**
```c
EdsError EdsWrite(EdsStreamRef inStream, EdsUInt32 inWriteSize, const EdsUInt8 *inBuffer, EdsUInt32 *outWrittenSize)
```

**Parameters**
- `inStream`: Stream object
- `inWriteSize`: Size to write
- `inBuffer`: Buffer to write
- `outWrittenSize`: Actual size written

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.38 EdsSeek
**Description**
Moves the read or write position of the stream (that is, the file position indicator).

**Syntax**
```c
EdsError EdsSeek(EdsStreamRef inStream, EdsInt32 inOffset, EdsSeekOrigin inOrigin, EdsUInt32 *outPosition)
```

**Parameters**
- `inStream`: Stream object
- `inOffset`: Offset to seek
- `inOrigin`: Seek origin
- `outPosition`: New position

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.39 EdsGetPosition
**Description**
Gets the current read or write position of the stream.

**Syntax**
```c
EdsError EdsGetPosition(EdsStreamRef inStream, EdsUInt32 *outPosition)
```

**Parameters**
- `inStream`: Stream object
- `outPosition`: Current position

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.40 EdsGetLength
**Description**
Gets the stream size.

**Syntax**
```c
EdsError EdsGetLength(EdsStreamRef inStream, EdsUInt32 *outLength)
```

**Parameters**
- `inStream`: Stream object
- `outLength`: Stream length

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.41 EdsCopyData
**Description**
Copies data from the copy source stream to the copy destination stream.

**Syntax**
```c
EdsError EdsCopyData(EdsStreamRef inCopySource, EdsStreamRef inCopyDestination)
```

**Parameters**
- `inCopySource`: Source stream
- `inCopyDestination`: Destination stream

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

## Image Operations

### 3.1.42 EdsCreateImageRef
**Description**
Creates an image object from an image file.

**Syntax**
```c
EdsError EdsCreateImageRef(EdsStreamRef inStream, EdsImageRef *outImageRef)
```

**Parameters**
- `inStream`: Stream object
- `outImageRef`: Image object

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.43 EdsGetImageInfo
**Description**
Gets image information from a designated image object.

**Syntax**
```c
EdsError EdsGetImageInfo(EdsImageRef inImageRef, EdsImageInfo *outImageInfo)
```

**Parameters**
- `inImageRef`: Image object
- `outImageInfo`: Image information structure

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.44 EdsGetImage
**Description**
Gets designated image data from an image file, in the form of a designated rectangle.

**Syntax**
```c
EdsError EdsGetImage(EdsImageRef inImageRef, EdsImageSource inSource, EdsUInt32 inSize, EdsRect inRect, EdsImageType inImageType, EdsStreamRef outStream)
```

**Parameters**
- `inImageRef`: Image object
- `inSource`: Image source
- `inSize`: Output size
- `inRect`: Rectangle area
- `inImageType`: Output image type
- `outStream`: Output stream

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

## Event Handlers

### 3.1.45 EdsSetCameraAddedHandler
**Description**
Registers a callback function for when a camera is detected.

**Syntax**
```c
EdsError EdsSetCameraAddedHandler(EdsCameraAddedFunc inFunction, void *inContext)
```

**Parameters**
- `inFunction`: Callback function
- `inContext`: User context

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.46 EdsSetObjectEventHandler
**Description**
Registers a callback function for receiving status change notification events for objects on a remote camera.

**Syntax**
```c
EdsError EdsSetObjectEventHandler(EdsCameraRef inCamera, EdsUInt32 inEvent, EdsObjectEventFunc inFunction, void *inContext)
```

**Parameters**
- `inCamera`: Camera object
- `inEvent`: Event type
- `inFunction`: Callback function
- `inContext`: User context

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.47 EdsSetPropertyEventHandler
**Description**
Registers a callback function for receiving status change notification events for property states on a camera.

**Syntax**
```c
EdsError EdsSetPropertyEventHandler(EdsCameraRef inCamera, EdsUInt32 inEvent, EdsPropertyEventFunc inFunction, void *inContext)
```

**Parameters**
- `inCamera`: Camera object
- `inEvent`: Event type
- `inFunction`: Callback function
- `inContext`: User context

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.48 EdsSetCameraStateEventHandler
**Description**
Registers a callback function for receiving status change notification events for camera objects.

**Syntax**
```c
EdsError EdsSetCameraStateEventHandler(EdsCameraRef inCamera, EdsUInt32 inEvent, EdsStateEventFunc inFunction, void *inContext)
```

**Parameters**
- `inCamera`: Camera object
- `inEvent`: Event type
- `inFunction`: Callback function
- `inContext`: User context

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.49 EdsSetProgressCallback
**Description**
Register a progress callback function for long-running operations.

**Syntax**
```c
EdsError EdsSetProgressCallback(EdsBaseRef inRef, EdsProgressFunc inFunction, void *inContext)
```

**Parameters**
- `inRef`: Object reference
- `inFunction`: Callback function
- `inContext`: User context

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

## Advanced Functions

### 3.1.50 EdsGetEvent
**Description**
This function acquires an event from a camera. In a console application, please call this function regularly to acquire an event from a camera.

**Syntax**
```c
EdsError EdsGetEvent(EdsCameraRef inCamera, EdsEvent *outEvent)
```

**Parameters**
- `inCamera`: Camera object
- `outEvent`: Event structure

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.51 EdsSetFramePoint
**Description**
Specifies the camera's focus and zoom frame position in the LiveView state.

**Syntax**
```c
EdsError EdsSetFramePoint(EdsCameraRef inCamera, EdsPoint inPoint)
```

**Parameters**
- `inCamera`: Camera object
- `inPoint`: Frame point coordinates

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

### 3.1.52 EdsSetMetaImage
**Description**
Writes information to the metadata of an image (JPEG only) in the camera.

**Syntax**
```c
EdsError EdsSetMetaImage(EdsCameraRef inCamera, EdsImageRef inImageRef)
```

**Parameters**
- `inCamera`: Camera object
- `inImageRef`: Image object

**Return Values**
Returns EDS_ERR_OK if successful. In other cases, see the EDS Error Lists.

---

## 3.2 EDS Error Lists

This section provides comprehensive error code lists organized by category. For detailed error code definitions and meanings, please refer to the EDSDK header file `EdsError.h`.

### 3.2.1 General errors
### 3.2.2 File access errors
### 3.2.3 Directory errors
### 3.2.4 Property errors
### 3.2.5 Function parameter errors
### 3.2.6 Device errors
### 3.2.7 Stream errors
### 3.2.8 Communication errors
### 3.2.9 Camera UI lock/unlock errors
### 3.2.10 STI/WIA errors
### 3.2.11 Other general errors
### 3.2.12 PTP errors
### 3.2.13 TakePicture errors

For complete error code listings and descriptions, please consult the original EDSDK documentation or the header files.

---

**Next:** [Asynchronous Events](04-Asynchronous-Events.md) | **Previous:** [Overview](02-Overview.md)