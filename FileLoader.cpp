#include "FileLoader.hpp"

namespace Aleg {
  std::string FileLoader::loadFile(std::string) {
    std::ifstream file(path);
    std::stringstream buffer;

    buffer << file.rdbuf();

    return buffer.str();
  }
}
