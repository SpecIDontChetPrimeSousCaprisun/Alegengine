#include <Alegengine/rendering/ScreenEffect.hpp>

namespace Aleg {
  std::map<std::string, ScreenEffect*> ScreenEffect::effects;

  ScreenEffect::ScreenEffect(std::string vertexPath, std::string fragPath, std::string mapName) 
    : shader(new Shader(vertexPath, fragPath)) {
    effects[mapName] = this;
  }

  ScreenEffect::ScreenEffect(const char* vertexSrc, const char* fragSrc, std::string mapName) 
    : shader(new Shader(vertexSrc, fragSrc)) {
    effects[mapName] = this;
  }

  void ScreenEffect::passShaderInfo() {
    glUniform1i(
      glGetUniformLocation(shader->program, "scene"),
      0
    );
  }
}
