#pragma once

#include <chrono>
#include <string>

namespace ExifWriter {

bool writeCommentAndDate(const std::string& imagePath,
                         const std::string& comment, int star_rating = 0);

}  // namespace ExifWriter