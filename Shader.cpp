#include "rendering/Shader.hpp"
#include "FileLoader.hpp"

namespace Aleg {
  Shader::Shader(std::string vertexPath, std::string fragPath) {
    const char* vertexSource = loadSource(vertexPath);
    const char* fragSource = loadSource(fragPath);

    unsigned int vertexShader = getShaderFromSource(vertexSource, GL_VERTEX_SHADER);
    unsigned int fragShader = getShaderFromSource(fragPath, GL_FRAGMENT_SHADER);

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);

    glLinkProgram(shaderProgram);

    glGetProgramiv(
        shaderProgram,
        GL_LINK_STATUS,
        &success
    );

    if (!success) {
      glGetProgramInfoLog(
          shaderProgram,
          512,
          NULL,
          infoLog
      );

      std::cout << "PROGRAM LINK ERROR:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
  }

  const char* Shader::loadSource(std::string path) {
    std::string code = FileLoader::loadFile(path);
    return code.c_str();
  }

  unsigned int getShaderFromSource(const char* source, GLenum type) {
    unsigned int shader glCreateShader(type);

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

      std::cout << "VERTEX SHADER ERROR:\n" << infoLog << std::endl;
    }

    return shader;
  }
}
