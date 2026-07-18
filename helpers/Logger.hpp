#pragma once

#include <string>

namespace Aleg {
  class Logger {
  public:
    Logger(std::string name);

    void log(std::string text);
    void warn(std::string text);
    void error(std::string text);
  private:
    std::string name;
  };
}
