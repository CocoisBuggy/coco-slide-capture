# Properties

Properties of camera and images objects can be retrieved and set by means of EdsGetPropertyData, EdsSetPropertyData, and other APIs.

For certain properties, if the target object is a camera, you can use the EdsGetPropertyDesc API to get the properties that can currently be set. For details, see the description of EdsGetPropertyDesc in the API Reference chapter.

For various properties that exist, this section explains the objects they describe and what properties mean.

## 5.1 Property Lists

### Camera Setting Properties (0x00000000 - 0x000000FF)

#### Basic Device Information
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_ProductName` | Camera product name/model | String |
| `kEdsPropID_BodyIDEx` | Extended body identification | String |
| `kEdsPropID_OwnerName` | Camera owner name | String |
| `kEdsPropID_Artist` | Artist name (metadata) | String |
| `kEdsPropID_Copyright` | Copyright information | String |
| `kEdsPropID_MakerName` | Manufacturer name (Canon) | String |
| `kEdsPropID_DateTime` | Current camera date/time | String |
| `kEdsPropID_FirmwareVersion` | Firmware version | String |

#### Storage & File Management
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_SaveTo` | Save destination (Camera/Host/Both) | UInt32 |
| `kEdsPropID_CurrentStorage` | Current storage device | UInt32 |
| `kEdsPropID_CurrentFolder` | Current folder path | String |

#### Battery & Power
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_BatteryLevel` | Battery charge level | UInt32 |
| `kEdsPropID_BatteryQuality` | Battery quality/condition | UInt32 |

### Capture Properties (0x00000400 - 0x000004FF)

#### Exposure Control
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_AEMode` | Auto Exposure mode (P/Tv/Av/M etc.) | UInt32 |
| `kEdsPropID_AEModeSelect` | AE mode selection | UInt32 |
| `kEdsPropID_DriveMode` | Drive mode (single/continuous) | UInt32 |
| `kEdsPropID_ISOSpeed` | ISO sensitivity | UInt32 |
| `kEdsPropID_MeteringMode` | Metering mode | UInt32 |
| `kEdsPropID_ExposureCompensation` | Exposure compensation | Int32 |
| `kEdsPropID_AvailableShots` | Number of available shots | UInt32 |

#### Focus & Lens Control
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_AFMode` | Auto Focus mode | UInt32 |
| `kEdsPropID_Av` | Aperture value | UInt32 |
| `kEdsPropID_Tv` | Shutter speed (Time value) | UInt32 |
| `kEdsPropID_FocalLength` | Focal length | UInt32 |
| `kEdsPropID_LensName` | Lens name/model | String |
| `kEdsPropID_LensStatus` | Lens status/information | UInt32 |
| `kEdsPropID_FocusInfo` | Focus information/AF points | FocusInfo |

#### Bracketing & Advanced Features
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_Bracket` | Bracketing settings | UInt32 |
| `kEdsPropID_AEBracket` | Auto Exposure bracketing | UInt32 |
| `kEdsPropID_FEBracket` | Flash Exposure bracketing | UInt32 |
| `kEdsPropID_ISOBracket` | ISO bracketing | UInt32 |
| `kEdsPropID_WhiteBalanceBracket` | White balance bracketing | UInt32 |

#### Flash Control
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_FlashOn` | Flash on/off state | UInt32 |
| `kEdsPropID_FlashMode` | Flash mode | UInt32 |
| `kEdsPropID_RedEye` | Red-eye reduction | UInt32 |

#### Color & White Balance
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_WhiteBalance` | White balance mode | UInt32 |
| `kEdsPropID_ColorTemperature` | Color temperature (Kelvin) | UInt32 |
| `kEdsPropID_WhiteBalanceShift` | White balance fine-tuning | Point |
| `kEdsPropID_ColorSpace` | Color space (sRGB/AdobeRGB) | UInt32 |

#### Picture Style
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_PictureStyle` | Picture style setting | UInt32 |
| `kEdsPropID_PictureStyleDesc` | Picture style description | PictureStyleDesc |
| `kEdsPropID_PictureStyleCaption` | Picture style caption | String |

#### Image Quality & Format
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_ImageQuality` | Image quality setting (RAW/JPEG/HEIF) | UInt32 |
| `kEdsPropID_Orientation` | Image orientation | UInt32 |
| `kEdsPropID_ICCProfile` | ICC color profile | String |

#### Noise Reduction & Quality
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_NoiseReduction` | Noise reduction setting | UInt32 |

### EVF (Electronic Viewfinder) Properties (0x00000500 - 0x000005FF)

#### EVF Output & Mode
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_Evf_OutputDevice` | EVF output destination | UInt32 |
| `kEdsPropID_Evf_Mode` | EVF mode | UInt32 |
| `kEdsPropID_Evf_DepthOfFieldPreview` | Depth of field preview | UInt32 |
| `kEdsPropID_Record` | Recording status | UInt32 |

#### EVF Color Settings
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_Evf_WhiteBalance` | EVF white balance | UInt32 |
| `kEdsPropID_Evf_ColorTemperature` | EVF color temperature | UInt32 |

#### EVF Image Control
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_Evf_Zoom` | EVF zoom ratio | UInt32 |
| `kEdsPropID_Evf_ZoomPosition` | EVF zoom position | Point |
| `kEdsPropID_Evf_ZoomRect` | EVF zoom rectangle | Rect |
| `kEdsPropID_Evf_ImagePosition` | EVF image position | Point |
| `kEdsPropID_Evf_CoordinateSystem` | EVF coordinate system | UInt32 |
| `kEdsPropID_Evf_VisibleRect` | EVF visible rectangle | Rect |

#### EVF Focus Control
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_Evf_AFMode` | EVF AF mode | UInt32 |

#### EVF Histogram
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_Evf_HistogramY` | EVF luminance histogram | UInt32_Array |
| `kEdsPropID_Evf_HistogramR` | EVF red histogram | UInt32_Array |
| `kEdsPropID_Evf_HistogramG` | EVF green histogram | UInt32_Array |
| `kEdsPropID_Evf_HistogramB` | EVF blue histogram | UInt32_Array |
| `kEdsPropID_Evf_HistogramStatus` | EVF histogram status | UInt32 |

#### Power Zoom Control
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_Evf_PowerZoom_CurPosition` | Current power zoom position | UInt32 |
| `kEdsPropID_Evf_PowerZoom_MaxPosition` | Maximum power zoom position | UInt32 |
| `kEdsPropID_Evf_PowerZoom_MinPosition` | Minimum power zoom position | UInt32 |

#### EVF Advanced Features
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_Evf_RollingPitching` | EVF rolling/pitching data | ByteBlock |

### GPS Properties (0x00000800 - 0x000008FF)

#### Location & Position
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_GPSVersionID` | GPS version information | String |
| `kEdsPropID_GPSLatitudeRef` | Latitude reference (N/S) | String |
| `kEdsPropID_GPSLatitude` | Latitude coordinates | Rational |
| `kEdsPropID_GPSLongitudeRef` | Longitude reference (E/W) | String |
| `kEdsPropID_GPSLongitude` | Longitude coordinates | Rational |
| `kEdsPropID_GPSAltitudeRef` | Altitude reference | UInt8 |
| `kEdsPropID_GPSAltitude` | Altitude value | Rational |

#### GPS Timing & Status
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_GPSTimeStamp` | GPS timestamp | Rational |
| `kEdsPropID_GPSSatellites` | Number of GPS satellites | String |
| `kEdsPropID_GPSStatus` | GPS status | String |
| `kEdsPropID_GPSMapDatum` | GPS map datum | String |
| `kEdsPropID_GPSDateStamp` | GPS date stamp | String |

### Advanced/Limited Properties (0x01000000+)

#### Time & Date Settings
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_UTCTime` | UTC time | Time |
| `kEdsPropID_TimeZone` | Time zone | String |
| `kEdsPropID_SummerTimeSetting` | Daylight saving time | UInt32 |

#### Movie Settings
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_FixedMovie` | Fixed movie settings | UInt32 |
| `kEdsPropID_MovieParam` | Movie parameters | UInt32 |
| `kEdsPropID_Aspect` | Aspect ratio | UInt32 |
| `kEdsPropID_StillMovieDivideSetting` | Still/movie divide setting | UInt32 |
| `kEdsPropID_CardExtension` | Card extension | UInt32 |
| `kEdsPropID_MovieCardExtension` | Movie card extension | UInt32 |
| `kEdsPropID_StillCurrentMedia` | Still current media | UInt32 |
| `kEdsPropID_MovieCurrentMedia` | Movie current media | UInt32 |

#### Temperature & Device Status
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_TempStatus` | Temperature status | UInt32 |

#### Mirror Lock & Advanced Features
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_MirrorUpSetting` | Mirror lock-up setting | UInt32 |
| `kEdsPropID_MirrorLockUpState` | Mirror lock-up state | UInt32 |

#### AF & Focus Advanced
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_AFEyeDetect` | AF eye detection | UInt32 |
| `kEdsPropID_FocusShiftSetting` | Focus shift setting | UInt32 |
| `kEdsPropID_ContinuousAfMode` | Continuous AF mode | UInt32 |
| `kEdsPropID_MovieServoAf` | Movie servo AF | UInt32 |
| `kEdsPropID_AFTrackingObject` | AF tracking object | UInt32 |
| `kEdsPropID_RegisterFocusEdge` | Register focus edge | UInt32 |
| `kEdsPropID_DriveFocusToEdge` | Drive focus to edge | UInt32 |
| `kEdsPropID_FocusPosition` | Focus position | UInt32 |

#### Power & Display Settings
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_AutoPowerOffSetting` | Auto power off setting | UInt32 |
| `kEdsPropID_ScreenOffTime` | Screen off time | UInt32 |
| `kEdsPropID_ScreenDimmerTime` | Screen dimmer time | UInt32 |
| `kEdsPropID_ViewfinderOffTime` | Viewfinder off time | UInt32 |

#### Lens & Camera Control
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_ApertureLockSetting` | Aperture lock setting | UInt32 |
| `kEdsPropID_LensIsSetting` | Lens IS setting | UInt32 |

#### Special Features
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_ColorFilter` | Color filter setting | UInt32 |
| `kEdsPropID_DigitalZoomSetting` | Digital zoom setting | UInt32 |
| `kEdsPropID_ShutterType` | Shutter type | UInt32 |
| `kEdsPropID_BrightnessSetting` | Brightness setting | UInt32 |

### DC (Digital Camera) Properties (0x00000600+)

| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_DC_Zoom` | DC zoom setting | UInt32 |
| `kEdsPropID_DC_Strobe` | DC strobe/flash | UInt32 |
| `kEdsPropID_LensBarrelStatus` | Lens barrel status | UInt32 |

#### Power Zoom Control
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_PowerZoom_Speed` | Power zoom speed | UInt32 |

#### Movie HFR (High Frame Rate)
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsPropID_MovieHFRSetting` | Movie high frame rate setting | UInt32 |

#### Meta Image Control
| Property ID | Description | Data Type |
|-------------|-------------|------------|
| `kEdsSetMetaImage` | Sets meta image information | Special API |

## Most Commonly Used Properties

For quick reference, here are the most frequently accessed properties:

### Essential Camera Control
- **`kEdsPropID_AEMode`** - Shooting mode (P/Tv/Av/M)
- **`kEdsPropID_ISOSpeed`** - ISO sensitivity
- **`kEdsPropID_Av`** - Aperture
- **`kEdsPropID_Tv`** - Shutter speed
- **`kEdsPropID_ImageQuality`** - Image quality setting

### Live View Control
- **`kEdsPropID_Evf_OutputDevice`** - Enable live view to PC
- **`kEdsPropID_Evf_Mode`** - Live view mode
- **`kEdsPropID_Evf_Zoom`** - Live view zoom

### Basic Information
- **`kEdsPropID_ProductName`** - Camera model
- **`kEdsPropID_BatteryLevel`** - Battery status
- **`kEdsPropID_SaveTo`** - Where to save images

### Focus & Exposure
- **`kEdsPropID_AFMode`** - Autofocus mode
- **`kEdsPropID_ExposureCompensation`** - Exposure compensation
- **`kEdsPropID_WhiteBalance`** - White balance setting

## Usage Notes

1. **Property Access**: Use `EdsGetPropertyData()` to read and `EdsSetPropertyData()` to write property values
2. **Property Size**: Always check property size and data type with `EdsGetPropertySize()` first
3. **Availability**: Not all properties are available on all camera models
4. **Events**: Register for property change events using `EdsSetPropertyEventHandler()`
5. **Error Handling**: Always check return codes from property operations

---

**Next:** [README](README.md) | **Previous:** [Asynchronous Events](04-Asynchronous-Events.md)