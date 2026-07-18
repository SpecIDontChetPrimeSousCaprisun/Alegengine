#pragma once 

#include "Logger.hpp"

#include <string>
#include <fstream>

namespace Aleg {
  class FileLoader {
  public:
    static std::string loadFile(std::string path);
    static unsigned int loadTexture(std::string path);
  private:
    static Logger* logger;
  };
}
