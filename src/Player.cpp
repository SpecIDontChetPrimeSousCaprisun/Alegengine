#include <Alegengine/objects/Player.hpp>
#include <Alegengine/objects/Particle.hpp>
#include <Alegengine/rendering/Window.hpp>

namespace Aleg {
  Player::Player(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, float zIndex, Window* window)
    : Object(position, size, transparency, color, zIndex, window) {
    collisionGroup = CollisionGroups::Player;
  }

  Player::Player(glm::vec2 position, glm::vec2 size, float transparency, const unsigned char* tex, unsigned int len, float zIndex, Window* window)
    : Object(position, size, transparency, tex, len, zIndex, window) {
    collisionGroup = CollisionGroups::Player;
  }

  void Player::beforeUpdate() {
    if (glfwGetKey(window->window, GLFW_KEY_D) == GLFW_PRESS) {
      linearVelocity.x = 250.0f * speedMult;
      //flipH = false;
    } else if (glfwGetKey(window->window, GLFW_KEY_A) == GLFW_PRESS) {
      linearVelocity.x = -250.0f * speedMult;
      //flipH = true;
    } else if ((linearVelocity.x == -250.0f * speedMult || linearVelocity.x == 250.0f * speedMult)) {
      linearVelocity.x = 0.0f;
    }

    if (type == "topdown") {
      if (glfwGetKey(window->window, GLFW_KEY_W) == GLFW_PRESS) {
        linearVelocity.y = -250.0f * speedMult;
      } else if (glfwGetKey(window->window, GLFW_KEY_S) == GLFW_PRESS) {
        linearVelocity.y = 250.0f * speedMult;
      } else if ((linearVelocity.y == -250.0f * speedMult || linearVelocity.y == 250.0f * speedMult)) {
        linearVelocity.y = 0.0f;
      }
    } else if (state == "idle" && glfwGetKey(window->window, GLFW_KEY_SPACE) == GLFW_PRESS) {
      linearVelocity.y -= 500.0f;
      state = "jumping";
      lastJump = 0.1f;
      Particle::spawnParticles(realPosition + glm::vec2(realSize.x / 2.0f, realSize.y), 
                               glm::vec2(50.0f, 50.0f),
                               0.0f,
                               glm::vec3(1.0f, 0.0f, 0.0f),
                               0.0f,
                               glm::vec2(0.0f, -500.0f),
                               100.0f,
                               2.0f,
                               5000.0f,
                               window);
    }

    std::vector<Object*> result = Object::getObjectsInBounds(realPosition + glm::vec2(0.0f, realSize.y),
                                                             glm::vec2(realSize.x, 1.0f),
                                                             0.0f,
                                                             window);

    if (state == "jumping") {
      if (!result.empty() && lastJump <= 0.0f) state = "idle";
      else lastJump -= window->deltaTime;
    } else if (state == "idle") {
      if (result.empty()) state = "jumping";
    }
  }
}
