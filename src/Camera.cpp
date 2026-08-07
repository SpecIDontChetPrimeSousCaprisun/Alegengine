#include <Alegengine/rendering/Camera.hpp>
#include <Alegengine/rendering/Window.hpp>

namespace Aleg {
  Camera::Camera(glm::vec2 pos) 
    : Object(pos, glm::vec2(0.0f, 0.0f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), -99999999) {
    anchored = true;
    canCollide = false;
  }

  DrawInfo* Camera::beforeDrawing() {
    return new DrawInfo(realPosition, realSize);
  }

  void Camera::beforeUpdate() {
    if (getParent()) {
      position = -glm::vec2(window->fbWidth, window->fbHeight) / 2.0f;
      position += getParent()->realSize / 2.0f;
    }
  }
}
