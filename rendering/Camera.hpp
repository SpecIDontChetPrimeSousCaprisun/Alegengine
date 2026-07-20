#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Aleg {
  class Camera {
  public:
    static Camera* currentCamera;

    Camera(glm::vec2 position);

    glm::vec2 position;
  };
}
