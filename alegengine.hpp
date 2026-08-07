#pragma once

#include "rendering/Window.hpp"
#include "rendering/Shader.hpp"
#include "rendering/Camera.hpp"
#include "rendering/ScreenEffect.hpp"
#include "rendering/PointLight.hpp"

#include "objects/Object.hpp"
#include "objects/ParentObject.hpp"
#include "objects/DrawInfo.hpp"
#include "objects/MaskResult.hpp"
#include "objects/CollisionGroup.hpp"
#include "objects/Player.hpp"
#include "objects/Particle.hpp"

#include "userInterface/UIElement.hpp"
#include "userInterface/TextElement.hpp"
#include "userInterface/Button.hpp"
#include "userInterface/ScrollingElement.hpp"
#include "userInterface/DragElement.hpp"

#include "helpers/FileLoader.hpp"
#include "helpers/Logger.hpp"
#include "helpers/Font.hpp"
#include "helpers/Tween.hpp"

#include <string>
#include <random>

namespace Aleg {
  extern std::string type;
  extern std::mt19937 rng;
  extern float baseLighting;

  void init(std::string type = "topdown"); // Types can be : topdown and side
  void mainLoop();
}
