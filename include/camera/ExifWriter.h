#pragma once

#include <chrono>
#include <string>

namespace ExifWriter {

bool writeCommentAndDate(const std::string& imagePath,
                         const std::string& comment);

}  // namespace ExifWriter