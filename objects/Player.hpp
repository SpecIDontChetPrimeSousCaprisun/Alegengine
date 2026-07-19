#pragma once

#include "Object.hpp"

namespace Aleg {
  class Player : public Object {
  public:
    Player(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, float zIndex);
    Player(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, float zIndex);

    float speedMult = 1.0f;
  protected:
    void beforeUpdate() override;

    float lastJump = 0.0f;
    std::string state = "jumping";
  };
}
