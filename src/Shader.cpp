#include "../rendering/Shader.hpp"
#include "../FileLoader.hpp"

#include <iostream>

namespace Aleg {
  Shader::Shader(std::string vertexPath, std::string fragPath) {
    std::string vertexCode = FileLoader::loadFile(vertexPath);
    std::string fragCode = FileLoader::loadFile(fragPath);

    const char* vertexSource = vertexCode.c_str();
    const char* fragSource = fragCode.c_str();

    unsigned int vertexShader = getShaderFromSource(vertexSource, GL_VERTEX_SHADER);
    unsigned int fragShader = getShaderFromSource(fragSource, GL_FRAGMENT_SHADER);

    int success;
    char infoLog[512];
    program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragShader);

    glLinkProgram(program);

    glGetProgramiv(
        program,
        GL_LINK_STATUS,
        &success
    );

    if (!success) {
      glGetProgramInfoLog(
          program,
          512,
          NULL,
          infoLog
      );

      std::cout << "PROGRAM LINK ERROR:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
  }

  unsigned int Shader::getShaderFromSource(const char* source, GLenum type) {
    unsigned int shader = glCreateShader(type);

    glShaderSource(
      shader,
      1,
      &source,
      NULL
    );

    glCompileShader(shader);

    int success;
    char infoLog[512];

    glGetShaderiv(
      shader,
      GL_COMPILE_STATUS,
      &success
    );

    if (!success) {
      glGetShaderInfoLog(
        shader,
        512,
        NULL,
        infoLog
      );

      std::cout << "SHADER ERROR:\n" << infoLog << std::endl;
    }

    return shader;
  }
}
