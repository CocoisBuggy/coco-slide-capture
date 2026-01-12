#include "camera/ExifWriter.h"

#include <chrono>
#include <ctime>
#include <exiv2/exiv2.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace ExifWriter {

bool writeCommentAndDate(const std::string& imagePath,
                         const std::string& comment, int star_rating,
                         const std::string& context_date) {
  try {
    // Load the image
    Exiv2::Image::UniquePtr image = Exiv2::ImageFactory::open(imagePath);
    if (!image) {
      std::cerr << "Failed to open image: " << imagePath << std::endl;
      return false;
    }

    // Read existing metadata
    image->readMetadata();
    Exiv2::ExifData& exifData = image->exifData();

    // Add date to DateTimeOriginal and DateTime fields
    auto dateTimeOrig =
        exifData.findKey(Exiv2::ExifKey("Exif.Photo.DateTimeOriginal"));
    auto dateTime = exifData.findKey(Exiv2::ExifKey("Exif.Image.DateTime"));

    std::string dateToUse;
    if (!context_date.empty()) {
      // Use context date if provided (format: YYYY-MM-DD)
      dateToUse = context_date + " 12:00:00";  // Use noon as default time
    } else {
      // Use current date if no context date provided
      auto now = std::chrono::system_clock::now();
      auto time_t = std::chrono::system_clock::to_time_t(now);
      std::tm tm = *std::localtime(&time_t);
      std::ostringstream dateStream;
      dateStream << std::put_time(&tm, "%Y:%m:%d %H:%M:%S");
      dateToUse = dateStream.str();
    }

    // Set both DateTimeOriginal and DateTime to ensure consistency
    if (dateTimeOrig == exifData.end()) {
      exifData["Exif.Photo.DateTimeOriginal"] = dateToUse;
    }
    if (dateTime == exifData.end()) {
      exifData["Exif.Image.DateTime"] = dateToUse;
    }

    // Add comment to ImageDescription
    if (!comment.empty()) {
      exifData["Exif.Image.ImageDescription"] = comment;
      exifData["Exif.Photo.UserComment"] = comment;
    }

    // Add star rating to EXIF data
    if (star_rating >= 0 && star_rating <= 5) {
      exifData["Exif.Image.Rating"] = uint16_t(star_rating);
      exifData["Exif.Image.RatingPercent"] =
          uint16_t(star_rating * 20);  // 0-100 scale
    }

    // Write the metadata back to the image
    image->writeMetadata();

    std::cout << "Successfully wrote EXIF data to " << imagePath << std::endl;
    return true;

  } catch (const std::exception& e) {
    std::cerr << "Error writing EXIF data: " << e.what() << std::endl;
    return false;
  }
}

}  // namespace ExifWriter