#include <Alegengine/alegengine.hpp>

namespace Aleg {
  Camera* Camera::currentCamera = nullptr;

  Camera::Camera(glm::vec2 pos) 
    : Object(pos, glm::vec2(0.0f, 0.0f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), -99999999) {
    anchored = true;
    canCollide = false;
  }

  DrawInfo* Camera::beforeDrawing() {
    return new DrawInfo(realPosition, realSize);
  }

  void Camera::beforeUpdate() {
    if (parent) {
      position = -glm::vec2(Aleg::Window::fbWidth, Aleg::Window::fbHeight) / 2.0f;
      position += parent->realSize / 2.0f;
    }
  }
}
