#pragma once

#include <Alegengine/userInterface/UIElement.hpp>

namespace Aleg {
  class DragElement : public UIElement {
  public:
    DragElement(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, float zIndex, Window* window = nullptr);
    DragElement(glm::vec2 position, glm::vec2 size, float transparency, const unsigned char* tex, unsigned int len, float zIndex, Window* window = nullptr);

    bool followX = true;
    bool followY = true;

    glm::vec2 topLeftBounds = glm::vec2(0.0f, 0.0f);
    glm::vec2 bottomRightBounds = glm::vec2(1.0f, 1.0f);
  protected:
    void beforeUpdate() override;

    bool clicked = false;
  };
}
