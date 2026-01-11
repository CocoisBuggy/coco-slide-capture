#include "FileManager.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>

FileManager::FileManager()
    : m_outputDirectory("./captures")
    , m_sequenceNumber(1) {
}

FileManager::~FileManager() {
}

void FileManager::setOutputDirectory(const std::string& directory) {
    m_outputDirectory = directory;
    
    // Create directory if it doesn't exist
    std::filesystem::create_directories(m_outputDirectory);
}

void FileManager::setContextDate(const std::optional<std::string>& date) {
    m_contextDate = date;
}

bool FileManager::downloadImage(EdsDirectoryItemRef item) {
    if (!item) {
        return false;
    }
    
    // Get directory item info
    EdsDirectoryItemInfo info;
    EdsError err = EdsGetDirectoryItemInfo(item, &info);
    if (err != EDS_ERR_OK) {
        std::cerr << "Failed to get directory item info: " << err << std::endl;
        return false;
    }
    
    // Generate filename
    std::string filename = generateFilename();
    std::string fullPath = m_outputDirectory + "/" + filename;
    
    // Download the file
    EdsStreamRef stream = nullptr;
    err = EdsCreateFileStream(fullPath.c_str(), kEdsFileCreateDisposition_CreateAlways, kEdsAccess_ReadWrite, &stream);
    if (err != EDS_ERR_OK) {
        std::cerr << "Failed to create file stream: " << err << std::endl;
        return false;
    }
    
    err = EdsDownload(item, info.size, stream);
    if (err != EDS_ERR_OK) {
        std::cerr << "Failed to download image: " << err << std::endl;
        EdsRelease(stream);
        return false;
    }
    
    EdsDownloadComplete(item);
    EdsRelease(stream);
    
    m_sequenceNumber++;
    std::cout << "Image downloaded to: " << fullPath << std::endl;
    return true;
}

std::string FileManager::generateFilename() const {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y%m%d_%H%M%S");
    
    if (m_contextDate) {
        ss << "_" << *m_contextDate;
    }
    
    ss << "_" << std::setfill('0') << std::setw(3) << m_sequenceNumber << ".cr2";
    
    return ss.str();
}

EdsError EDSCALLBACK FileManager::downloadProgressCallback(EdsUInt32 progress, EdsUInt32 complete, EdsVoid* context) {
    // Handle progress updates if needed
    return EDS_ERR_OK;
}