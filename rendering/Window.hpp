#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

namespace Aleg {
  class Window {
  public:
    static int init();
    static void mainLoop();
    
    static GLFWwindow* window;
    static bool inGame;
    static int fbWidth;
    static int fbHeight;
  };
}
