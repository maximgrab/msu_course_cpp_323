#pragma once

#include <fstream>
#include <string>

namespace uni_cource_cpp {

class Logger {
 public:
  static Logger& get_instance() {
    static Logger* singleton = new Logger();
    return *singleton;
  }

  void log(const std::string& string);

  static void set_log_path(const std::string& new_log_path);

 private:
  Logger() : ofs_(log_path_ + "log.txt") {}
  ~Logger() {
    if (ofs_)
      ofs_.close();
  }

  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;

  std::ofstream ofs_;
  static std::string log_path_;
};

}  // namespace uni_cource_cpp
