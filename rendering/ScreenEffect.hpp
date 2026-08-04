#pragma once

#include <Alegengine/rendering/Shader.hpp>
#include <map>
#include <string>

namespace Aleg {
  class ScreenEffect {
  public:
    ScreenEffect(std::string vertexPath, std::string fragPath, std::string mapName);
    ScreenEffect(const char* vertexSrc, const char* fragSrc, std::string mapName);

    static std::map<std::string, ScreenEffect*> effects;

    void passShaderInfo();

    Shader* shader;
  };
}
