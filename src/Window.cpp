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
  std::vector<std::function<void(Window*)>> Window::frameCallbacks;
  std::vector<std::function<void(Window*, double, double)>> Window::scrollCallbacks;

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

    glfwSetWindowUserPointer(window, this);
    glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
      Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

      for (std::function<void(Window*, double, double)> callback : scrollCallbacks) {
        callback(self, xoffset, yoffset);
      }
    });

    // Effects
    float vertices[] = {
      // position      texcoords

      -1.0f,  1.0f,    0.0f, 1.0f,
      -1.0f, -1.0f,    0.0f, 0.0f,
      1.0f, -1.0f,    1.0f, 0.0f,

      -1.0f,  1.0f,    0.0f, 1.0f,
      1.0f, -1.0f,    1.0f, 0.0f,
      1.0f,  1.0f,    1.0f, 1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(vertices),
      vertices,
      GL_STATIC_DRAW
    );

    // position
    glVertexAttribPointer(
      0,
      2,
      GL_FLOAT,
      GL_FALSE,
      4 * sizeof(float),
      (void*)0
    );
    glEnableVertexAttribArray(0);

    // UV
    glVertexAttribPointer(
      1,
      2,
      GL_FLOAT,
      GL_FALSE,
      4 * sizeof(float),
      (void*)(2 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    makeSceneTexture();
    makePingpongBuffers();

    Window::currentWindow = this;
    parent = new ParentObject();
    cam = new Camera(glm::vec2(0.0f, 0.0f));
    windows[mapName] = this;
  }

  void Window::makeSceneTexture() {
    glGenTextures(1, &sceneTexture);
    glBindTexture(GL_TEXTURE_2D, sceneTexture);

    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RGB,
      fbWidth,
      fbHeight,
      0,
      GL_RGB,
      GL_UNSIGNED_BYTE,
      nullptr
    );

    // Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenFramebuffers(1, &sceneFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);

    // create sceneTexture
    glFramebufferTexture2D(
      GL_FRAMEBUFFER,
      GL_COLOR_ATTACHMENT0,
      GL_TEXTURE_2D,
      sceneTexture,
      0
    );

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        logger->error("Framebuffer is incomplete!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void Window::makePingpongBuffers() {
    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongTexture);

    for (int i = 0; i < 2; i++) {
      glBindTexture(GL_TEXTURE_2D, pingpongTexture[i]);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fbWidth, fbHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

      glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongTexture[i], 0);

      if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        logger->error("Pingpong framebuffer is incomplete!");
      }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
      makeSceneTexture();
      makePingpongBuffers();
      parent->recursivelyRecalculateFonts();
    }

    if (!screenEffects.empty()) {
      glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);
      glClear(GL_COLOR_BUFFER_BIT);

      glViewport(0, 0, fbWidth, fbHeight);
    }

    parent->recursivelyDrawChildren(); // Draw

    if (!screenEffects.empty()) {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glViewport(0, 0, fbWidth, fbHeight);
      glClear(GL_COLOR_BUFFER_BIT);

      bool horizontal = true;
      unsigned int inputTexture = sceneTexture; // first pass reads the original scene

      for (size_t i = 0; i < screenEffects.size(); i++) {
        ScreenEffect* effect = screenEffects[i];
        bool isLastEffect = (i == screenEffects.size() - 1);

        // Last effect writes to the real screen; others write to a ping-pong buffer
        glBindFramebuffer(GL_FRAMEBUFFER, isLastEffect ? 0 : pingpongFBO[horizontal]);
        if (!isLastEffect) glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(effect->shader->program);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, inputTexture); // read from PREVIOUS pass's output

        effect->passShaderInfo();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        inputTexture = pingpongTexture[horizontal]; // next effect reads what we just wrote
        horizontal = !horizontal;
      }
    }
    for (std::function<void(Window*)> func : frameCallbacks) {
      func(this);
    }

    glfwSwapBuffers(window);
  }
}
