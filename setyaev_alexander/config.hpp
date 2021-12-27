#pragma once
namespace uni_course_cpp{
namespace config {

inline constexpr const char* kTempDirectoryPath = "./setyaev_alexander/temp/";
inline constexpr const char* kLogFilename = "log.txt";
inline const std::string kLogFilePath = std::string(kTempDirectoryPath) + std::string(kLogFilename);

}
}
