#include <Alegengine/userInterface/DragElement.hpp>
#include <Alegengine/rendering/Window.hpp>
#include <algorithm>

namespace Aleg {
  DragElement::DragElement(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, float zIndex, Window* window) 
    : UIElement(position, size, transparency, color, zIndex, window) {}

  DragElement::DragElement(glm::vec2 position, glm::vec2 size, float transparency, const unsigned char* tex, unsigned int len, float zIndex, Window* window) 
    : UIElement(position, size, transparency, tex, len, zIndex, window) {}

  void DragElement::beforeUpdate() {
    recalculateFocus();
    if (clicked) {
      if (!(glfwGetMouseButton(window->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)) {
        clicked = false;
        return;
      }
    } else if (focused && glfwGetMouseButton(window->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) clicked = true;
    else return;

    double mouseX;
    double mouseY;

    glfwGetCursorPos(window->window, &mouseX, &mouseY);
    glm::vec2 trueParentSize = getParent()->realSize * glm::vec2(window->fbWidth, window->fbHeight);
    glm::vec2 localisedMouse = glm::vec2((mouseX - trueParentSize.x) / trueParentSize.x,
                                         (mouseY - trueParentSize.y) / trueParentSize.y);

    if (followX) {
      position.x = std::clamp(localisedMouse.x, topLeftBounds.x, bottomRightBounds.x);
    }

    if (followY) {
      position.y = std::clamp(localisedMouse.y, topLeftBounds.y, bottomRightBounds.y);
    }
  }
}
