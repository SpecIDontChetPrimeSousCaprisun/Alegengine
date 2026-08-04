#include <Alegengine/alegengine.hpp>

namespace Aleg {
  Logger* Button::logger = new Logger("Button");

  Button::Button(glm::vec2 position,
                 glm::vec2 size,
                 float transparency,
                 glm::vec3 color,
                 float zIndex,
                 const unsigned char* font,
                 unsigned int fontLen,
                 std::string text,
                 Window* window)
    : TextElement(position, size, transparency, color, zIndex, font, fontLen, text, window) {initObject();}

  Button::Button(glm::vec2 position,
                 glm::vec2 size,
                 float transparency,
                 const unsigned char* tex,
                 unsigned int len,
                 float zIndex,
                 const unsigned char* font,
                 unsigned int fontLen,
                 std::string text,
                 Window* window)
    : TextElement(position, size, transparency, tex, len, zIndex, font, fontLen, text, window) {initObject();}

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
