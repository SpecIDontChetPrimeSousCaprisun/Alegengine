#pragma once 

#include <Alegengine/rendering/Window.hpp>
#include "Logger.hpp"

#include <string>
#include <fstream>
#include <vector>

namespace Aleg {
  class FileLoader {
  public:
    static std::string loadFile(std::string path);
    static unsigned int loadTexture(const unsigned char* tex, unsigned int len, Window* window);
    static std::vector<unsigned char> loadFontFile(const std::string& path);
  private:
    static Logger* logger;
  };
}
