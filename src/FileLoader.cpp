#include "../helpers/FileLoader.hpp"

#include <fstream>
#include <sstream>

namespace Aleg {
  std::string FileLoader::loadFile(std::string path) {
    std::ifstream file(path);
    std::stringstream buffer;

    buffer << file.rdbuf();

    return buffer.str();
  }
}
