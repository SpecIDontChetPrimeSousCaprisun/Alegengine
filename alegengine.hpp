#pragma once

#include "rendering/Window.hpp"
#include "rendering/Shader.hpp"
#include "rendering/Camera.hpp"

#include "objects/Object.hpp"
#include "objects/ParentObject.hpp"
#include "objects/DrawInfo.hpp"
#include "objects/MaskResult.hpp"
#include "objects/CollisionGroup.hpp"
#include "objects/Player.hpp"

#include "userInterface/UIElement.hpp"
#include "userInterface/TextElement.hpp"
#include "userInterface/Button.hpp"
#include "userInterface/ScrollingElement.hpp"

#include "helpers/FileLoader.hpp"
#include "helpers/Logger.hpp"
#include "helpers/Font.hpp"

#include <string>

namespace Aleg {
  extern std::string type;

  void init(std::string type = "topdown"); // Types can be : topdown and side
  void mainLoop();
}
