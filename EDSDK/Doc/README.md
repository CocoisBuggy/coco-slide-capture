# EDSDK API Programming Reference

This documentation contains the complete EDSDK (EOS Digital SDK) API Programming Reference, broken down into organized chapters for easy navigation and reference.

## About EDSDK

EDSDK stands for **EOS Digital Camera Software Development Kit**. It provides the functions required to control cameras connected to a host PC, digital images created in digital cameras, and images downloaded to the PC. This SDK allows users to implement representative functions such as:

- Transfer of images from camera to host PC storage
- Remote camera control and image shooting from host PC
- Access to camera properties and live view functionality

## Supported Platforms

- **Windows**: Windows 10, 11 (64-bit/32-bit)
- **macOS**: Universal library, macOS v12-14 (64-bit)
- **Linux**: ARM32/ARM64 and x64 versions available

## Documentation Structure

### 📚 [Introduction](01-Introduction.md)
- Basic topics and concepts
- Supported environments (Windows, macOS, Linux)
- Complete list of supported cameras
- Installation and setup instructions

### 🏗️ [Overview](02-Overview.md)
- System architecture and protocols
- EDSDK objects and hierarchy
- Object management principles
- Asynchronous events concept
- Basic data types and error handling

### 🔧 [API Reference](03-API-Reference.md)
- Complete API function documentation (52 functions)
- Organized by functionality categories:
  - SDK Initialization & Management
  - Object Navigation
  - Camera & Device Information
  - Camera Sessions
  - Camera Commands
  - Property Management
  - File Operations
  - File Download Operations
  - Live View Operations
  - Stream Management
  - Stream I/O Operations
  - Image Operations
  - Event Handlers
  - Advanced Functions

### 📡 [Asynchronous Events](04-Asynchronous-Events.md)
- Object-related events
- Property-related events
- State-related events
- Complete event data specifications
- Error codes and handling

### ⚙️ [Properties](05-Properties.md)
- Comprehensive property reference
- Organized by categories:
  - Camera Settings
  - Capture Properties
  - EVF (Live View) Properties
  - GPS Properties
  - Advanced/Limited Properties
  - DC Properties

## Quick Reference

### Most Commonly Used APIs
| Function | Purpose |
|-----------|---------|
| `EdsInitializeSDK` | Initialize SDK |
| `EdsGetCameraList` | Get connected cameras |
| `EdsOpenSession` | Connect to camera |
| `EdsGetPropertyData` | Get property values |
| `EdsSetPropertyData` | Set property values |
| `EdsSendCommand` | Send camera commands |
| `EdsDownload` | Download files |
| `EdsDownloadEvfImage` | Get live view |

### Most Commonly Used Properties
| Property | Purpose |
|-----------|---------|
| `kEdsPropID_AEMode` | Shooting mode (P/Tv/Av) |
| `kEdsPropID_ISOSpeed` | ISO sensitivity |
| `kEdsPropID_Av` | Aperture |
| `kEdsPropID_Tv` | Shutter speed |
| `kEdsPropID_ImageQuality` | Image quality |
| `kEdsPropID_Evf_OutputDevice` | Live view output |
| `kEdsPropID_BatteryLevel` | Battery status |

## Supported Cameras (as of September 2024)

This SDK supports a wide range of Canon EOS and PowerShot cameras including:

### Latest Models
- EOS R1, EOS R5 Mark II, EOS R6 Mark II, EOS R8, EOS R50, EOS R100
- EOS R7, EOS R10, EOS R3, EOS R5, EOS R6
- PowerShot V10, PowerShot ZOOM

### Previous Generation Support
- EOS-1D X Mark III, EOS 5D Mark IV, EOS 6D Mark II
- EOS 80D, EOS 90D, EOS M6 Mark II, EOS M50 Mark II
- Plus many PowerShot G series models

*Note: Some older models have limited remote capture functionality.*

## Getting Started

### Basic Development Workflow
1. **Initialize**: Call `EdsInitializeSDK()`
2. **Connect**: Get camera list and open session
3. **Configure**: Set properties and register event handlers
4. **Control**: Send commands, handle events
5. **Download**: Transfer images as needed
6. **Cleanup**: Close session and terminate SDK

### Development Environment
- Include headers: `EDSDK.h`, `EDSDKTypes.h`, `EDSDKErrors.h`
- Link with appropriate library (EDSDK.dll/EDSDK.framework/libEDSDK.so)
- Handle asynchronous events properly
- Manage object reference counters

## Error Handling

All EDSDK functions return `EdsError` codes:
- `EDS_ERR_OK` - Success
- Other codes - Various error conditions (see API Reference)

Always check return values and handle errors gracefully.

## Cross-Platform Considerations

### Windows
- Requires COM initialization for multi-threading
- Use `CoInitializeEx()`/`CoUninitialize()` for additional threads

### macOS
- Run loop processing may be needed for camera detection on macOS 13+
- Universal binary supports both Intel and Apple Silicon

### Linux
- Ensure proper library paths for libEDSDK.so
- Tested on Ubuntu 18.04+ (various ARM/x64 configurations)

## Code Samples

Sample code is available in multiple languages:
- Swift (macOS)
- C# (Windows)
- Objective-C (macOS)
- C/C++ (cross-platform)

## Additional Resources

### Reference Materials
- [Library/docs samples](../Library/docs/) - Example implementations
- Header files - Complete type definitions and constants
- Error code listings in `EDSDKError.h`

### Support
For SDK-specific questions and issues, contact the Canon SDK support team in your region.

---

## Navigation

📖 **Start Reading**: [Introduction](01-Introduction.md) → 

🔄 **Sequential Reading**: Introduction → Overview → API Reference → Asynchronous Events → Properties

🔍 **Specific Topics**: Jump to relevant chapters using the links above

---

*This documentation is extracted from the original EDSDK 13.18.40 API Programming Reference, reorganized for better readability and navigation. Copyright © 2018-2024 Canon Inc.*