#include <Alegengine/alegengine.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>
#include <iomanip>
#include <iostream>

namespace Aleg {
  Logger* Window::logger = new Logger("Window");
  Window* Window::currentWindow = nullptr;
  std::map<std::string, Window*> Window::windows;
  std::vector<std::function<void()>> Window::frameCallbacks;
  std::vector<std::function<void(double, double)>> Window::scrollCallbacks;

  Window::Window(float width, float height, std::string name, std::string mapName)
    : fbWidth(width), fbHeight(height) {
    GLFWwindow* shareWith = currentWindow ? currentWindow->window : nullptr;
    window = glfwCreateWindow(width, height, name.c_str(), NULL, shareWith);

    if (!window) {
      glfwTerminate();
      logger->error("Failed to open window");
      return;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cerr << "Failed to initialize GLAD" << std::endl;
      return;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    /*glm::mat4 projection = glm::ortho(
        0.0f, (float)fbWidth,
        (float)fbHeight, 0.0f,
        -1.0f, 1.0f
    );*/
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
      for (std::function<void(double, double)> callback : scrollCallbacks) {
        callback(xoffset, yoffset);
      }
    });

    Window::currentWindow = this;
    parent = new ParentObject();
    cam = new Camera(glm::vec2(0.0f, 0.0f));
    windows[mapName] = this;
  }

  int Window::init() {
    if (!glfwInit()) return -1;

    currentWindow = new Window(600, 400, "Game", "main");

    return 0;
  }

  void Window::mainLoop() {
    while (true) {
      bool hasUpdated = false;

      Object::deletePendingObjects();

      for (auto& [name, window] : windows) {
        if (!window->shouldUpdate || glfwWindowShouldClose(window->window)) {
          glfwDestroyWindow(window->window);
          windows.erase(name);
        }

        hasUpdated = true;
        window->update();
      }

      if (!hasUpdated) break;
    }

    glfwTerminate();
  }

  void Window::update() {
    glfwMakeContextCurrent(window);
    currentWindow = this;

    glfwPollEvents();

    double currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (deltaTime > 0.1) deltaTime = 0.1;

    int oldFbWidth = fbWidth;
    int oldFbHeight = fbHeight;

    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);
    glClear(GL_COLOR_BUFFER_BIT); 

    parent->update();
    parent->recursivelyUpdateChildren();

    if (fbWidth != oldFbWidth || fbHeight != oldFbHeight) {
      parent->recursivelyRecalculateFonts();
    }

    parent->recursivelyDrawChildren();

    for (std::function<void()> func : frameCallbacks) {
      func();
    }

    glfwSwapBuffers(window);
  }
}
