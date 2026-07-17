#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

namespace Aleg {
  class Shader {
  public:
    Shader(std::string vertexPath, std::string fragPath);

    unsigned int program;
  private:
    static const char* loadSource(std::string path);
    static unsigned int getShaderFromSource(const char* source, GLenum type);
  }
}
