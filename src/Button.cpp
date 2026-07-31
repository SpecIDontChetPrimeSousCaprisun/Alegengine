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
    recalculateFocus();

    if (focused) {
      colorChange = glm::vec3(-0.1f, -0.1f, -0.1f);

      if (glfwGetMouseButton(window->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && (!pressed || retriggerOnEchoes)) {
        pressed = true;
        callback();
      } else if (glfwGetMouseButton(window->window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS) {
        pressed = false;
      }
    } else {
      pressed = false;
      colorChange = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    if (pressed) colorChange = glm::vec3(0.1f, 0.1f, 0.1f);
  }
}
