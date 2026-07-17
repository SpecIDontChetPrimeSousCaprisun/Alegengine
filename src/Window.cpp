#include "../rendering/Window.hpp"
#include "../objects/Object.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>
#include <iomanip>
#include <iostream>

namespace Aleg {
  GLFWwindow* Window::window = nullptr;
  bool Window::inGame = true;
  int Window::fbWidth = 600;
  int Window::fbHeight = 480;
  double Window::deltaTime = 0;
  double Window::lastFrame = 0;

  int Window::init() {
    if (!glfwInit()) return -1;

    window = glfwCreateWindow(fbWidth, fbHeight, "Alegengine", NULL, NULL);

    if (!window) {
      glfwTerminate();
      return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cerr << "Failed to initialize GLAD" << std::endl;
      return -1;
    }

    glClearColor(0.2f, 0.4f, 0.6f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    /*glm::mat4 projection = glm::ortho(
        0.0f, (float)fbWidth,
        (float)fbHeight, 0.0f,
        -1.0f, 1.0f
    );*/
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    return 0;
  }

  void Window::mainLoop() {
    while (inGame && !glfwWindowShouldClose(window)) {
      glfwPollEvents();

      double currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;

      if (deltaTime > 0.1) deltaTime = 0.1;


      glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
      glViewport(0, 0, fbWidth, fbHeight);
      glClear(GL_COLOR_BUFFER_BIT);

      // Update
      Object::updateAll();

      // Draw
      Object::drawAll();

      glfwSwapBuffers(window);
    }

    glfwTerminate();
  }
}
