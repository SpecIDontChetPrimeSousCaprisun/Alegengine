#pragma once

#include <Alegengine/objects/Object.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

namespace Aleg {
  class PointLight : public Object {
  public:
    PointLight(glm::vec2 position, float radius, float intensity, glm::vec3 color, Window* window = nullptr);

    float radius;
    float intensity;
    glm::vec3 color;
  };
}
