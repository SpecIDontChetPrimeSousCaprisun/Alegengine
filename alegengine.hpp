#pragma once

#include "rendering/Window.hpp"

#include <string>

namespace Aleg {
  void init(std::string type = "topdown"); // Types can be : topdown and side
  void mainLoop();
}
