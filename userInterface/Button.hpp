#pragma once

#include <Alegengine/userInterface/TextElement.hpp>
#include <Alegengine/helpers/Logger.hpp>
#include <functional>

namespace Aleg {
  class Button : public TextElement {
  public:
    Button(glm::vec2 position,
           glm::vec2 size,
           float transparency,
           glm::vec3 color,
           float zIndex,
           const unsigned char* font,
           unsigned int fontLen,
           std::string text,
           Window* window = nullptr);

    Button(glm::vec2 position,
           glm::vec2 size,
           float transparency,
           const unsigned char* tex,
           unsigned int len,
           float zIndex,
           const unsigned char* font,
           unsigned int fontLen,
           std::string text,
           Window* window = nullptr);

    std::function<void()> callback;
    bool retriggerOnEchoes = false;
  protected:
    void beforeUpdate() override;
  private:
    static Logger* logger;

    void initObject();
  
    bool pressed = false;
  };
}
