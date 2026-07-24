#pragma once

#include <Alegengine/objects/Object.hpp>

namespace Aleg {
  class UIElement : public Object {
  public:
    UIElement(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, float zIndex);
    UIElement(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, float zIndex);
  protected:
    DrawInfo* beforeDrawing() override;
  private:
    void initObject();
  };
}
