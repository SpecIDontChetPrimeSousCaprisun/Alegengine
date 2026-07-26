#include <Alegengine/alegengine.hpp>

namespace Aleg {
  Logger* Button::logger = new Logger("Button");

  Button::Button(glm::vec2 position,
                 glm::vec2 size,
                 float transparency,
                 glm::vec3 color,
                 float zIndex,
                 std::string fontPath,
                 std::string text)
    : TextElement(position, size, transparency, color, zIndex, fontPath, text) {initObject();}

  Button::Button(glm::vec2 position,
                 glm::vec2 size,
                 float transparency,
                 std::string texPath,
                 float zIndex,
                 std::string fontPath,
                 std::string text)
    : TextElement(position, size, transparency, texPath, zIndex, fontPath, text) {initObject();}

  void Button::initObject() {
    callback = []() {
      logger->log("Pressed !");
    };
  }

  void Button::beforeUpdate() {
    double mouseX;
    double mouseY;

    glfwGetCursorPos(Window::window, &mouseX, &mouseY);

    DrawInfo* info = beforeDrawing();

    if (info->position.x < mouseX &&
        info->position.x + info->size.x > mouseX &&
        info->position.y < mouseY &&
        info->position.y + info->size.y > mouseY) {
      colorChange = glm::vec3(-0.1f, -0.1f, -0.1f);

      if (glfwGetMouseButton(Window::window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && (!pressed || retriggerOnEchoes)) {
        pressed = true;
        callback();
      } else if (glfwGetMouseButton(Window::window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS) {
        pressed = false;
      }
    } else {
      pressed = false;
      colorChange = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    if (pressed) colorChange = glm::vec3(0.1f, 0.1f, 0.1f);
  }
}
