#pragma once

#include <Alegengine/helpers/Logger.hpp>
#include <Alegengine/rendering/Camera.hpp>
#include <Alegengine/objects/ParentObject.hpp>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <map>
#include <functional>
#include <string>

namespace Aleg {
  class Window {
  public:
    Window(float width, float height, std::string name, std::string mapName);

    GLFWwindow* window;
    ParentObject* parent;
    Camera* cam;
    bool shouldUpdate = true;
    int fbWidth;
    int fbHeight;
    double deltaTime = 0;
    double lastFrame = 0;

    static void mainLoop();
    static int init();

    static Logger* logger;
    static Window* currentWindow;
    static std::map<std::string, Window*> windows;
    static std::vector<std::function<void()>> frameCallbacks;
    static std::vector<std::function<void(double, double)>> scrollCallbacks;
  private:
    void update();
  };
}
