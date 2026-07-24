#include <Alegengine/alegengine.hpp>

namespace Aleg {
  UIElement::UIElement(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, float zIndex) 
    : Object(position, size, transparency, color, zIndex + 999) {
    initObject();
  }

  UIElement::UIElement(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, float zIndex) 
    : Object(position, size, transparency, texPath, zIndex + 999) {
    initObject();
  }

  void UIElement::initObject() {
    anchored = true;
    canCollide = false;
  }

  DrawInfo* UIElement::beforeDrawing() {
    return new DrawInfo(realPosition * glm::vec2(Window::fbWidth, Window::fbHeight), realSize * glm::vec2(Window::fbWidth, Window::fbHeight));
  }
}
