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
include_directories(include/camera)
include_directories(include/ui)
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
    src/camera/CameraManager.cpp
    src/camera/LiveViewRenderer.cpp
    src/ui/MainWindow.cpp
)

set(HEADERS
    include/camera/CameraManager.h
    include/camera/LiveViewRenderer.h
    include/ui/MainWindow.h
)

# Create executable
add_executable(film ${SOURCES} ${HEADERS})

# Clang-format target
find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT} -i ${SOURCES} ${HEADERS}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Formatting source files with clang-format"
    )
endif()

# Link libraries
target_link_libraries(film PRIVATE 
    edsdk
    ${GTK4_LIBRARIES}
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
