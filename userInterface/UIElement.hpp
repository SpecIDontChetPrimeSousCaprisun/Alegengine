#pragma once

#include <Alegengine/objects/Object.hpp>

namespace Aleg {
  class UIElement : public Object {
  public:
    UIElement(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, float zIndex, Window* window = nullptr);
    UIElement(glm::vec2 position, glm::vec2 size, float transparency, const unsigned char* tex, unsigned int len, float zIndex, Window* window = nullptr);

    bool getFocus();
  protected:
    void recalculateFocus();
    virtual void beforeUpdate() override;
    virtual void afterUpdate() override;

    DrawInfo* beforeDrawing() override;

    bool focused = false;
  private:
    void initObject();
  };
}
