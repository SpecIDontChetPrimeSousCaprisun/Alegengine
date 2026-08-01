#include <Alegengine/alegengine.hpp>

namespace Aleg {
  std::map<std::string, ScreenEffect*> ScreenEffect::effects;

  ScreenEffect::ScreenEffect(std::string vertexPath, std::string fragPath, std::string mapName) 
    : shader(new Shader(vertexPath, fragPath)) {
    effects[mapName] = this;
  }

  void ScreenEffect::passShaderInfo() {
    glUniform1i(
      glGetUniformLocation(shader->program, "scene"),
      0
    );
  }
}
