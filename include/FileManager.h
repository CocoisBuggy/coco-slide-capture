#pragma once

#include <string>
#include <optional>
#include "EDSDK.h"

class FileManager {
public:
    FileManager();
    ~FileManager();
    
    void setOutputDirectory(const std::string& directory);
    void setContextDate(const std::optional<std::string>& date);
    
    bool downloadImage(EdsDirectoryItemRef item);
    std::string generateFilename() const;
    
    const std::string& getOutputDirectory() const { return m_outputDirectory; }
    const std::optional<std::string>& getContextDate() const { return m_contextDate; }
    
private:
    std::string m_outputDirectory;
    std::optional<std::string> m_contextDate;
    int m_sequenceNumber;
    
    static EdsError EDSCALLBACK downloadProgressCallback(EdsUInt32 progress, EdsUInt32 complete, EdsVoid* context);
};