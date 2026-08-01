#pragma once

#include <Alegengine/helpers/Logger.hpp>
#include <Alegengine/rendering/Camera.hpp>
#include <Alegengine/rendering/ScreenEffect.hpp>
#include <Alegengine/objects/ParentObject.hpp>
#include <Alegengine/objects/Player.hpp>
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
    ParentObject* parent = nullptr;
    Camera* cam = nullptr;
    bool shouldUpdate = true;
    int fbWidth;
    int fbHeight;
    double deltaTime = 0;
    double lastFrame = 0;
    std::vector<ScreenEffect*> screenEffects;

    static void mainLoop();
    static int init();

    static Logger* logger;
    static Window* currentWindow;
    static std::map<std::string, Window*> windows;
    static std::vector<std::function<void(Window*)>> frameCallbacks;
    static std::vector<std::function<void(Window*, double, double)>> scrollCallbacks;
  private:
    void update();
    void makeSceneTexture();
    void makePingpongBuffers();

    unsigned int VAO, VBO, sceneTexture, sceneFBO;
    unsigned int pingpongFBO[2];
    unsigned int pingpongTexture[2];
  };
}
