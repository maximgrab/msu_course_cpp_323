#pragma once

#include <fstream>
#include <mutex>
namespace uni_course_cpp {
class Logger {
 public:
  static Logger& getLogger() {
    static Logger logger;
    return logger;
  }
  ~Logger();
  void log(const std::string& log_string);

 private:
  std::mutex log_mutex_;
  Logger();
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  std::ofstream file_;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;
};
}  // namespace uni_course_cpp
