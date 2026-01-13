# Asynchronous Events

In the case of asynchronous events, notify the host computer of changes, such as changes in the state of properties of remote cameras.

To enable an application to receive issued events, you must prepare callback functions for event reception and register them in the EDSDK by means of EdsSetPropertyEventHandler, EdsSetObjectEventHandler, EdsSetCameraStateEventHandler, EdsSetCameraAddedHandler, EdsSetProgressCallback, or other APIs for configuring callback functions.

For details on callback function types, see the parameters information of the APIs for callback function configuration.

This section describes events that can be retrieved by callback functions registered using EdsSetPropertyEventHandler, EdsSetObjectEventHandler, and EdsSetCameraStateEventHandler in particular.

## 4.1 Event Lists

### 4.1.1 Object-related events
| Events |
|--------|
| Notification of file creation |
| Notification of file deletion |
| Notification of changes in file information |
| Notification of changes in the volume information of recording media |
| Notification of requests to update volume information |
| Notification of requests to update folder information |
| Notification of file transfer requests |
| Notification of direct transfer requests |
| Notification of requests to cancel direct transfer |

### 4.1.2 Property-related events
| Events |
|--------|
| Notification of property state changes |
| Notification of state changes in configurable property values |

### 4.1.3 State-related events
| Events |
|--------|
| Notification of camera disconnection |
| Notification of changes in job states |
| Notification of warnings when the camera will shut off |
| Notification that the camera will remain on for a longer period |
| Notification of remote release failure |
| Notification of internal SDK errors |
| Notification of inform condition of Power Zoom Adapter |

## 4.2 Event Details

Events are explained in the following format:

### 4.2.xx EventID
Event ID of the issued event. Used to distinguish event types in callback functions.

#### Description
Explains the event and cites related considerations.

#### Event Data
Event data passed as event callback function arguments.

| Event Data | Data Type | Argument Name in the Callback Function |
|------------|-----------|----------------------------------------|
| The nature of the data that is passed | The data type | The value passed as an argument |

---

### 4.2.1 kEdsStateEvent_Shutdown (Notification of camera disconnection)

#### Description
Indicates that a camera is no longer connected to a computer, whether it was disconnected by unplugging a cord, opening the compact flash compartment, turning the camera off, auto shut-off, or by other means.

#### Event Data
| Event Data | Data Type | Value of inParameter |
|-----------|-----------|----------------------|
| None | – | – |

---

### 4.2.2 kEdsPropertyEvent_PropertyChanged (Notification of property state changes)

#### Description
Notifies that a camera property value has been changed. The changed property can be retrieved from event data. The changed value can be retrieved by means of EdsGetPropertyData.

If the property type is 0x0000FFFF, the changed property cannot be identified. Thus, retrieve all required properties repeatedly.

#### Event Data
| Event Data | Data Type | Value of inPropertyID |
|-----------|-----------|-----------------------|
| The property type | EdsPropertyID | A property ID |

#### See Also
- For details on property IDs, see the Property Lists.

---

### 4.2.3 kEdsPropertyEvent_PropertyDescChanged (Notification of state changes in configurable property values)

#### Description
Notifies of changes in the list of camera properties with configurable values. The list of configurable values for property IDs indicated in event data can be retrieved by means of EdsGetPropertyDesc.

#### Event Data
| Event Data | Data Type | Value of inPropertyID |
|-----------|-----------|-----------------------|
| Property type for which the list of configurable values has changed | EdsPropertyID | Of the capture-related properties, those properties that have configurable values that can be retrieved; otherwise, "Unknown" (0x0000FFFF) |

#### See Also
- For details on property IDs, see the Property Lists.

---

### 4.2.4 kEdsObjectEvent_DirItemCreated (Notification of file creation)

#### Description
Notifies of the creation of objects such as new folders or files on a camera compact flash card or the like. This event is generated if the camera has been set to store captured images simultaneously on the camera and a computer, for example, but not if the camera is set to store images on the computer alone. Newly created objects are indicated by event data.

#### Event Data
| Event Data | Data Type | Value of inRef |
|-----------|-----------|----------------|
| New directory or file object | EdsDirectoryItemRef | Pointer to the directory or file object |

---

### 4.2.5 kEdsObjectEvent_DirItemRemoved (Notification of file deletion)

#### Description
Notifies of the deletion of objects such as folders or files on a camera compact flash card or the like. Deleted objects are indicated in event data.

#### Event Data
| Event Data | Data Type | Value of inRef |
|-----------|-----------|----------------|
| Deleted directory or file object | EdsDirectoryItemRef | Pointer to the directory or file object |

---

### 4.2.6 kEdsObjectEvent_DirItemInfoChanged (Notification of changes in file information)

#### Description
Notifies that information of DirItem objects has been changed. Changed objects are indicated by event data. The changed value can be retrieved by means of EdsGetDirectoryItemInfo.

#### Event Data
| Event Data | Data Type | Value of inRef |
|-----------|-----------|----------------|
| Changed directory or file object | EdsDirectoryItemRef | Pointer to the directory or file object |

---

### 4.2.7 kEdsObjectEvent_DirItemContentChanged

#### Description
Notifies that header information has been updated, as for rotation information of image files on the camera. If this event is received, get the file header information again, as needed.

#### Event Data
| Event Data | Data Type | Value of inRef |
|-----------|-----------|----------------|
| Changed file | EdsDirectoryItemRef | Pointer to the directory item object |

#### Note
- To retrieve image properties, you must obtain them from image objects after using DownloadImage or DownloadThumbnail.

---

### 4.2.8 kEdsObjectEvent_VolumeInfoChanged (Notification of changes in the volume information of recording media)

#### Description
Notifies that the volume object (memory card) state (VolumeInfo) has been changed. Changed objects are indicated by event data. The changed value can be retrieved by means of EdsGetVolumeInfo.

#### Event Data
| Event Data | Data Type | Value of inRef |
|-----------|-----------|----------------|
| Changed volume object | EdsVolumeRef | Pointer to the volume object |

---

### 4.2.9 kEdsObjectEvent_VolumeUpdateItems (Notification of requests to update volume information)

#### Description
Notifies if the designated volume on a camera has been formatted. If notification of this event is received, get sub-items of the designated volume again as needed. Changed volume objects can be retrieved from event data.

#### Event Data
| Event Data | Data Type | Value of inRef |
|-----------|-----------|----------------|
| Changed volume object | EdsVolumeRef | Pointer to the volume object |

---

### 4.2.10 kEdsObjectEvent_FolderUpdateItems (Notification of requests to update folder information)

#### Description
Notifies if many images are deleted in a designated folder on a camera. If notification of this event is received, get sub-items of the designated folder again as needed. Changed folders (specifically, directory item objects) can be retrieved from event data.

#### Event Data
| Event Data | Data Type | Value of inRef |
|-----------|-----------|----------------|
| Changed folder | EdsDirectoryItemRef | Pointer to the directory item object |

---

### 4.2.11 kEdsStateEvent_JobStatusChanged (Notification of changes in job states)

#### Description
Notifies of whether or not there are objects waiting to be transferred to a host computer. This is useful when ensuring all shot images have been transferred when the application is closed.

#### Event Data
| Event Data | Data Type | Value of inParameter |
|-----------|-----------|----------------------|
| Whether or not there are objects waiting to be transferred | EdsUInt32 | 1: There are objects to be transferred<br>0: There are no objects to be transferred |

---

### 4.2.12 kEdsObjectEvent_DirItemRequestTransfer (Notification of file transfer requests)

#### Description
Notifies that there are objects on a camera to be transferred to a computer. This event is generated after remote release from a computer or local release from a camera. If this event is received, objects indicated in the event data must be downloaded. Furthermore, if the application does not require the objects, instead of downloading them, execute EdsDownloadCancel and release resources held by the camera.

#### Event Data
| Event Data | Data Type | Value of inRef |
|-----------|-----------|----------------|
| Array of directories or file objects to be transferred | EdsDirectoryItemRef | Directory or file object |

---

### 4.2.13 kEdsObjectEvent_DirItemRequestTransferDT (Notification of direct transfer requests)

#### Description
Notifies if the camera's direct transfer button is pressed. If this event is received, objects indicated in the event data must be downloaded. Furthermore, if the application does not require the objects, instead of downloading them, execute EdsDownloadCancel and release resources held by the camera.

#### Event Data
| Event Data | Data Type | Value of inRef |
|-----------|-----------|----------------|
| Array of directories or file objects to be transferred directly | EdsDirectoryItemRef | Array of directories and file objects |

---

### 4.2.14 kEdsObjectEvent_DirItemCancelTransferDT (Notification of requests to cancel direct transfer)

#### Description
Notifies of requests from a camera to cancel object transfer if the button to cancel direct transfer is pressed on the camera. If the parameter is 0, it means that cancellation of transfer is requested for objects still not downloaded, with these objects indicated by kEdsObjectEvent_DirItemRequestTransferDT.

#### Event Data
| Event Data | Data Type | Value of inRef |
|-----------|-----------|----------------|
| Array of directories or file objects for which to cancel transfer | EdsDirectoryItemRef [] | Array of directories and file objects |

---

### 4.2.15 kEdsStateEvent_WillSoonShutDown (Notification of warnings when the camera will shut off)

#### Description
Notifies that the camera will shut down after a specific period. Generated only if auto shut-off is set. Exactly when notification is issued (that is, the number of seconds until shutdown) varies depending on the camera model. To continue operation without having the camera shut down, use EdsSendCommand to extend the auto shut-off timer. The time in seconds until the camera shuts down is returned as the initial value.

#### Event Data
| Event Data | Data Type | Value of inParameter |
|-----------|-----------|----------------------|
| Number of seconds until the camera shuts down | EdsUint32 | Number of seconds |

---

### 4.2.16 kEdsStateEvent_ShutDownTimerUpdate (Notification that the camera will remain on for a longer period)

#### Description
As the counterpart event to kEdsStateEvent_WillSoonShutDown, this event notifies of updates to the number of seconds until a camera shuts down. After the update, the period until shutdown is model-dependent.

#### Event Data
| Event Data | Data Type | Value of inParameter |
|-----------|-----------|----------------------|
| None | – | – |

---

### 4.2.17 kEdsStateEvent_CaptureError (Notification of remote release failure)

#### Description
Notifies that a requested release has failed, due to focus failure or similar factors.

#### Event Data
| Event Data | Data Type | Value of inParameter |
|-----------|-----------|----------------------|
| Error code | EdsUint32 | Error code |

#### Error Codes
| Error Code | Description |
|------------|-------------|
| 0x00000001 | Shooting failure |
| 0x00000002 | The lens was closed |
| 0x00000003 | General errors from the shooting mode, such as errors from the bulb or mirror-up mechanism |
| 0x00000004 | Sensor cleaning |
| 0x00000005 | Error because the camera was set for silent operation |
| 0x00000006 | Card not inserted |
| 0x00000007 | Card error (including CARD-FULL/No.-FULL) |
| 0x00000008 | Write-protected |

---

### 4.2.18 kEdsStateEvent_InternalError (Notification of internal SDK errors)

#### Description
Notifies of internal SDK errors. If this error event is received, the issuing device will probably not be able to continue working properly, so cancel the remote connection.

#### Event Data
| Event Data | Data Type | Value of inParameter |
|-----------|-----------|----------------------|
| – | EdsUint32 | Unspecified value |

---

### 4.2.19 kEdsStateEvent_PowerZoomInfoChanged (Notification of inform condition of Power Zoom Adapter)

#### Description
Inform condition of Power Zoom Adapter. This event inform is guaranteed during Live View mode.

#### Event Data
| Event Data | Data Type | Value of inParameter |
|-----------|-----------|----------------------|
| Error code | EdsUint32 | Condition of Power Zoom Adapter (this is shown as bit assign. Please refer below information.) |

#### Detail of Value of inParameter

| Bit number | Description | Value |
|------------|-------------|-------|
| 0 | controllable yes/no | 1: enable to control zoom position of Power Zoom Adapter<br>0: disable to control zoom position of Power Zoom Adapter |
| 1 | PZ/MZ switch condition | 1: switch position is PZ (Power Zoom)<br>0: switch position is MZ (Manual Zoom) |
| 2 | condition of zoom drive condition | 1: drive lens by Power Zoom Adapter<br>0: not drive lens |
| 4~3 | Lens zoom position is tele position yes/no | 01: zoom position is TELE position<br>11: zoom position is WIDE position<br>10: zoom position is other position |
| 5 | attached condition | 1: Power Zoom Adapter is attached correctly<br>0: others |
| 6 | Battery Life | 1: battery life is enough<br>0: battery life isn't enough / less, or not attached battery |
| 7 | condition of lens zoom lock | 1: lens zoom ring isn't fixed/locked.<br>0: lens zoom ring is fixed/locked. |
| 8 | Condition of zoom speed change switch of body | 1: switch condition is SLOW<br>0: switch condition is FAST<br><br>Note: this switch condition isn't affected to zoom speed control by SDK. |
| 9 | notification of internal temperature increases | 1: increase Power Zoom Adapter body internal temperature is increasing<br>0: others |
| 10 | notification body internal temperature (stop drive) | 1: internal temperature of Power Zoom Adapter is high. At this moment, zoom drive control is stopped.<br>0: others |
| 32~17 | zoom drive speed level | Current speed of zoom drive. "0" position means stopped (no movement) |

---

**Next:** [Properties](05-Properties.md) | **Previous:** [API Reference](03-API-Reference.md)