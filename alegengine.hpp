#pragma once

#include "rendering/Window.hpp"
#include "rendering/Shader.hpp"

#include "objects/Object.hpp"
#include "objects/DrawInfo.hpp"
#include "objects/CollisionGroup.hpp"

#include "helpers/FileLoader.hpp"
#include "helpers/Logger.hpp"

#include <string>

namespace Aleg {
  void init(std::string type = "topdown"); // Types can be : topdown and side
  void mainLoop();
}
