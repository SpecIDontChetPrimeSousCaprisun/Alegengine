#include "../helpers/Logger.hpp"
#include <iostream>

namespace Aleg {
  Logger::Logger(std::string name) : name(name) {}

  void Logger::log(std::string text) {
    #ifdef ALEG_DEBUG
    std::cout << "\033[32m[LOG] (" << name << ") : \033[0m" << text << "\n";
    #endif
  }

  void Logger::warn(std::string text) {
    #ifdef ALEG_DEBUG
    std::cout << "\033[33m[WARN] (" << name << ") : \033[0m" << text << "\n";
    #endif
  }

  void Logger::error(std::string text) {
    std::cout << "\033[31m[ERROR] (" << name << ") : \033[0m" << text << "\n";
  }
}
