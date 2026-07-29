#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <functional>

namespace Aleg {
  class Window {
  public:
    static int init();
    static void mainLoop();
    
    static GLFWwindow* window;
    static bool inGame;
    static int fbWidth;
    static int fbHeight;
    static double deltaTime;
    static double lastFrame;
    static std::vector<std::function<void()>> frameCallbacks;
    static std::vector<std::function<void(double, double)>> scrollCallbacks;
  };
}
