# Introduction

EDSDK stands for EOS Digital Camera Software Development Kit. EDSDK provides the functions required to control cameras connected to a host PC, digital images created in digital cameras, and images downloaded to the PC. This document describes the collection of functions implemented in the EDSDK library.

EDSDK provides an interface for accessing image data shot using a Canon digital camera. Using EDSDK allows users to implement the following types of representative functions in software:

- Allows transfer of images in a camera to storage media on a host PC.
- Allows remotely connected cameras and the image being shot to be controlled from a host PC.

## 1.1 Basic Topics

EDSDK provides a C language interface for accessing Canon digital cameras and data created by these cameras. EDSDK is designed to provide standard methods of accessing different camera models and their data. Using EDSDK allows users to implement Canon digital camera features in software.

## 1.2 Supported Environments

EDSDK can be used on system configurations such as shown in the table below.

### 1.2.1 EDSDK for Windows

Checked with the environment on Windows 10,11 (64bit/32bit)

### 1.2.2 EDSDK for macOS

Provided as a universal library.
Checked with the environment on macOS v12-14 (64bit)

**Notes:** Console apps are not guaranteed to work.
Apple Silicon Macs with macOS 13.0-13.2 installed can't connect to a camera. Please use macOS 13.3 or later.
In macOS 14.0-14.1, connection failures occur. Please use macOS 14.2 or later.

### 1.2.3 EDSDK for Linux (ARM32/ARM64)

Checked with the environment below.

| No | CPU | OS | Hardware |
|---|---|---|---|
| 1 | ARMv8 Cortex-A57 | Ubuntu 18.04 LTS (64bit) | Jetson Nano Developer Kit B01 |
| 2 | ARMv8 Cortex-A72 | Raspberry Pi OS version: 11 (bullseye) (32bit) | Raspberry Pi4 Model B (4GB) |

### 1.2.4 EDSDK for Linux (x64)

Checked with the environment on "Ubuntu 20.04 LTS (64bit)"

## 1.3 Supported Cameras

### 1.3.1 Supported Cameras

The following models are supported as of September 2024.

| Supported Cameras | Camera Control |
|---|---|
| EOS R1 | ✔ |
| EOS R5 Mark II | ✔ |
| PowerShot V10 (Firmware version 1.1.0 or later) | ✔ |
| EOS R100 | ✔ |
| PowerShot ZOOM (Firmware version 1.1.2 or later) | ✔ |
| EOS R50 | ✔ |
| EOS R8 | ✔ |
| EOS R6 Mark II | ✔ |
| EOS R10 | ✔ |
| EOS R7 | ✔ |
| EOS R3 | ✔ |
| EOS Kiss M2 / EOS M50 Mark II | ✔ |
| EOS R5 | ✔ |
| EOS R6 | ✔ |
| EOS Kiss X10i / EOS Rebel T8i / EOS 850D | ✔ |
| EOS Ra | ✔ |
| EOS-1D X Mark III | ✔ |
| EOS M200 | ✔ |
| EOS M6 Mark II | ✔ |
| EOS 90D | ✔ |
| PowerShot G7X Mark III | ✔ |
| PowerShot G5X Mark II | ✔ |
| EOS Kiss X10 / EOS Rebel SL3 / EOS 250D / EOS 200D II | ✔ |
| EOS RP | ✔ |
| PowerShot SX70 HS | ✔ |
| EOS R | ✔ |
| EOS Kiss M / EOS M50 | ✔ |
| EOS Kiss X90 / EOS REBEL T7 / EOS 2000D / EOS 1500D | ✔ |
| EOS REBEL T100/EOS 4000D / EOS 3000D | ✔ |
| EOS M100 | ✔ *1 |
| EOS 6D Mark II | ✔ |
| EOS Kiss X9 / EOS Rebel SL2 / EOS 200D | ✔ |
| EOS Kiss X9i / EOS Rebel T7i / EOS 800D | ✔ |
| EOS 9000D / EOS 77D | ✔ |
| EOS M6 | ✔ *1 |
| EOS M5 | ✔ *1 |
| EOS 5D Mark IV | ✔ |
| EOS-1D X Mark II | ✔ |
| EOS 80D | ✔ |
| EOS Kiss X80 / EOS Rebel T6 / EOS 1300D | ✔ |
| EOS M10 | ✔ *1 |
| EOS 5DS | ✔ |
| EOS 5DS R | ✔ |
| EOS 8000D / EOS REBEL T6sEOS 760D | ✔ |
| EOS Kiss X8i / EOS REBEL T6i / EOS 750D | ✔ |
| EOS M3 | ✔ *1 |
| EOS 7D Mark II | ✔ |
| EOS Kiss X70/EOS 1200D/EOS REBEL T5/EOS Hi | ✔ |
| EOS M2 | ✔ *1 |
| EOS 70D | ✔ |
| EOS Kiss X7 / EOS 100D / EOS REBEL SL1 | ✔ |
| EOS Kiss X7i / EOS 700D / EOS REBEL T5i | ✔ |
| EOS-1D C | ✔ |
| EOS 6D | ✔ |
| EOS M | ✔ *1 |
| EOS Kiss X6i / EOS 650D / EOS REBEL T4i | ✔ |
| EOS-1D X | ✔ |
| EOS 5D Mark III | ✔ |
| EOS Kiss X50 / EOS REBEL T3 / EOS 1100D | ✔ |
| EOS Kiss X5 / EOS REBEL T3i / EOS 600D | ✔ |
| EOS 60D | ✔ |
| EOS Kiss X4 / EOS REBEL T2i / EOS 550D | ✔ |
| EOS-1D Mark IV | ✔ |
| EOS 7D | ✔ |
| EOS Kiss X3 / EOS REBEL T1i / EOS 500D | ✔ |
| EOS 5D Mark II | ✔ |
| EOS 50D | ✔ |
| EOS DIGITAL REBEL XS / 1000D/ KISS F | ✔ |
| EOS DIGITAL REBEL Xsi / 450D / Kiss X2 | ✔ |
| EOS-1Ds Mark III | ✔ |
| EOS 40D | ✔ |
| EOS-1D Mark III | ✔ |

**Notes:** If you need to handle RAW images on your software, please contact SDK support team in your country.
*1 Remote capture functions are not supported.

## 1.4 Installing EDSDK

### 1.4.1 Including Header Files

The following files are required in order to use the EDSDK using C/C++ language:
- EDSDK.h
- EDSDKTypes.h
- EDSDKErrors.h

**Windows:**
Be sure to copy the three header files listed above into the header access folder of the development environment. Be sure to add them to the application project workspace.
*Since these are C language header files, it is necessary to provide header files depending on the programming language.

**Macintosh:**
Be sure to include the three header files listed above.

### 1.4.2 Linking the Library

After header files are included, it is necessary to link the EDSDK library as described below.

**Windows:**
There are two methods of linking EDSDK: one where EDSDK.lib files are copied to the folder specified by a development environment library path and EDSDK.lib is specified as an import module, and another where EDSDK.dll is loaded by the LoadLibrary function.
When loading EDSDK.dll, get pointers to each EDSDK function using the GetProcAddress function and assign them to function pointer variables. When calling each EDSDK function, make the call via the function pointer variable obtained here.

**Macintosh:**
Add EDSDK.framework to Groups＆Files.

**Linux:**
Link the shared library libEDSDK.so when compiling your application.

### 1.4.3 Executing the EDSDK Client Application

**Windows:**
All DLLs are required in order to execute an EDSDK client application.
All of the modules in the DLL folder must be copied into the same folder where the EDSDK client application is in.

**Notes:** Do not copy the collection of EDSDK library files to the system folder or extension folder.

**Macintosh:**
Place EDSDK.framework in an application directory such as Contents/frameworks/.
It is also possible to load "EDSDK.framework" as a source file.

**Notes:** Do not copy the EDSDK framework file to the system folder.

**Linux:**
Resolve the path to the shared library libEDSDK.so at application runtime.
**Notes:** Do not copy the EDSDK library files to the system folder.

---

**Next:** [Overview](02-Overview.md) | **Previous:** [Table of Contents](README.md)