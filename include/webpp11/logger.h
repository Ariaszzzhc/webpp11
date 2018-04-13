#ifndef WEBPP11_LOGGER_H
#define WEBPP11_LOGGER_H

#include <iostream>
#include <string>

namespace webpp {
class Logger {
 public:
  void warn(const std::string msg) {
    std::cout << "[WARN]: " << msg << std::endl;
  }
  void info(const std::string msg) {
    std::cout << "[INFO]: " << msg << std::endl;
  }
  void debug(const std::string msg) {
    std::cout << "[DEBUG]: " << msg << std::endl;
  }
  void trace(const std::string msg) {
    std::cout << "[TRACE]: " << msg << std::endl;
  }
};
}  // namespace webpp

#endif