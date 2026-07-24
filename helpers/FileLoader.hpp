#pragma once 

#include "Logger.hpp"

#include <string>
#include <fstream>
#include <vector>

namespace Aleg {
  class FileLoader {
  public:
    static std::string loadFile(std::string path);
    static unsigned int loadTexture(std::string path);
    static std::vector<unsigned char> loadFontFile(const std::string& path);
  private:
    static Logger* logger;
  };
}
