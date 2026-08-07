#include <Alegengine/rendering/PointLight.hpp>
#include <Alegengine/rendering/Window.hpp>

namespace Aleg {
  PointLight::PointLight(glm::vec2 position, float radius, float intensity, glm::vec3 color, Window* window) 
    : Object(position, glm::vec2(0.0f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, window),
      radius(radius), intensity(intensity), color(color) {
    anchored = true;
    canCollide = false;
    this->window->lights.push_back(this);
  }
}
