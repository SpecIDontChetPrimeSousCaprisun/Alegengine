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
           std::string fontPath,
           std::string text);

    Button(glm::vec2 position,
           glm::vec2 size,
           float transparency,
           std::string texPath,
           float zIndex,
           std::string fontPath,
           std::string text);

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
