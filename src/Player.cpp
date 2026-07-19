#include <Alegengine/alegengine.hpp>

namespace Aleg {
  Player::Player(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, float zIndex) 
    : Object(position, size, transparency, color, zIndex) {}

  Player::Player(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, float zIndex) 
    : Object(position, size, transparency, texPath, zIndex) {}

  void Player::beforeUpdate() {
    if (glfwGetKey(Window::window, GLFW_KEY_D) == GLFW_PRESS) {
      linearVelocity.x = 250.0f * speedMult;
      //flipH = false;
    } else if (glfwGetKey(Window::window, GLFW_KEY_A) == GLFW_PRESS) {
      linearVelocity.x = -250.0f * speedMult;
      //flipH = true;
    } else if ((linearVelocity.x == -250.0f * speedMult || linearVelocity.x == 250.0f * speedMult)) {
      linearVelocity.x = 0.0f;
    }

    if (type == "topdown") {
      if (glfwGetKey(Window::window, GLFW_KEY_W) == GLFW_PRESS) {
        linearVelocity.y = -250.0f * speedMult;
      } else if (glfwGetKey(Window::window, GLFW_KEY_S) == GLFW_PRESS) {
        linearVelocity.y = 250.0f * speedMult;
      } else if ((linearVelocity.y == -250.0f * speedMult || linearVelocity.y == 250.0f * speedMult)) {
        linearVelocity.y = 0.0f;
      }
    } else if (state == "idle" && glfwGetKey(Window::window, GLFW_KEY_SPACE) == GLFW_PRESS) {
      linearVelocity.y -= 500.0f;
      state = "jumping";
      lastJump = 0.1f;
    }

    if (state == "jumping") {
      RaycastResult* result = Object::raycast(realPosition + glm::vec2(realSize.x / 2, realSize.y),
                                              glm::vec2(0.0f, 1.0f));

      if (result && lastJump <= 0.0f) state = "idle";
      else lastJump -= Window::deltaTime;
    }
  }
}
