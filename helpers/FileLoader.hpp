#pragma once 

#include <string>
#include <fstream>

namespace Aleg {
  class FileLoader {
  public:
    static std::string loadFile(std::string path);
  };
}
