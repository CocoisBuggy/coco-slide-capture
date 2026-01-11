# Canon Camera Control Application Specification

## Overview
A C++ application that connects to Canon cameras to provide live view and automated photography capabilities.

## Core Features

### 1. Camera Connection
- Connect to Canon cameras using EDSDK
- Handle camera detection and initialization
- Manage connection state and disconnection

### 2. Live View
- Display real-time camera feed in a window
- Continuously update the view from camera data
- Handle live view start/stop properly

### 3. Capture Trigger
- Listen for spacebar press
- On spacebar:
  - Auto focus
  - Auto expose 
  - Take photo shot
  - Download raw file to local directory

### 4. File Management
- Save captured RAW files to a local directory
- The local directory is based on the current application context (the user specifies an active directory)
- The active directory is ascociated with the casette of film slides we are working on
- Organize files (timestamp or sequence naming)
- Handle download completion and errors

## Technical Requirements

### Dependencies
- EDSDK (Canon SDK)
- C++17 or later
- GTK4 for the UI
- Platform-specific handling for the EDSDK libraries

### Architecture
- Camera manager class for connection handling
- Live view renderer for display
- Event handler for keyboard input
- File downloader for RAW file handling
- Main application loop

### Error Handling
- Graceful handling of camera disconnection
- EDSDK error code translation
- UI feedback for connection status and capture status

### Performance
- Smooth live view rendering
- Minimal latency between spacebar press and capture
- Efficient memory management for image data

## User Experience
- Simple, clean interface showing live view
- Visual feedback when capture is in progress
- Status indicators for connection and focus state
- Easily change what dir the photos are saved to, we are scanning from many cassette and the user must be able to specify where these images go
- Change context date for a dir

### Context date

the application should have a context date. The context date is not always available
but is sometimes written on the film casette. When a date is available, the user should
be able to specify the context date of this casette and all images saved should have their
metadata updated to reflect this.

Whenever we move to another casette, the context date must reset.
