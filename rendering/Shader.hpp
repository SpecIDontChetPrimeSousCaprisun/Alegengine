#pragma once

#include "../helpers/Logger.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

namespace Aleg {
  class ShaderInfo {
  public:
    ShaderInfo(std::string name, const char* vertexSrc, const char* fragSrc);

    std::string name;
    const char* vertexSrc; 
    const char* fragSrc;
  };

  class Shader {
  public:
    Shader(std::string vertexPath, std::string fragPath);
    Shader(const char* vertexSrc, const char* fragSrc);

    unsigned int program;
  private:
    static Logger* logger;

    static unsigned int getShaderFromSource(const char* source, GLenum type);
  };
}
