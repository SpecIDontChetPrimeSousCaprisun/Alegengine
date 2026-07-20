#include <Alegengine/alegengine.hpp>

namespace Aleg {
  Camera* Camera::currentCamera = new Camera(glm::vec2(0.0f, 0.0f));

  Camera::Camera(glm::vec2 pos) : position(pos) {}
}
