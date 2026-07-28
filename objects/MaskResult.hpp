#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Aleg {
  class MaskResult {
  public:
    glm::vec2 position;
    glm::vec2 size;
    bool hasMask;
  };
}
