cmake_minimum_required(VERSION 3.10)
project(film CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

# Include directories
include_directories(include)
include_directories(EDSDK/Header)

# EDSDK library
set(EDSDK_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/EDSDK/Library/x86_64")
add_library(edsdk SHARED IMPORTED)
set_target_properties(edsdk PROPERTIES
    IMPORTED_LOCATION "${EDSDK_LIB_DIR}/libEDSDK.so"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/EDSDK/Header"
)

# Source files
set(SOURCES
    src/main.cpp
    src/CameraManager.cpp
    src/LiveViewRenderer.cpp
    src/EventHandler.cpp
    src/FileManager.cpp
)

set(HEADERS
    include/CameraManager.h
    include/LiveViewRenderer.h
    include/EventHandler.h
    include/FileManager.h
)

# Create executable
add_executable(film ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(film PRIVATE 
    edsdk
    ${GTK4_LIBRARIES}
    /nix/store/rs3qlyvyz8bzr9nx2diah7ds4vgsfcm0-steam-1.0.0.85-fhsenv-rootfs/usr/lib64/libusb-1.0.so
)

# Include directories
target_include_directories(film PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/EDSDK/Header
    ${GTK4_INCLUDE_DIRS}
)

# Add compatibility definitions
target_compile_definitions(film PRIVATE 
    TARGET_OS_LINUX
)
target_compile_options(film PRIVATE ${GTK4_CFLAGS_OTHER})
