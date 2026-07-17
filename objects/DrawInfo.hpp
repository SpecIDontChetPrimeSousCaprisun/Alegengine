#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Aleg {
  class DrawInfo {
  public:
    DrawInfo(glm::vec2 position, glm::vec2 size);

    glm::vec2 position;
    glm::vec2 size;
    bool shouldDraw = true;
  };
}
