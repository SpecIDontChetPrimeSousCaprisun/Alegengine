#pragma once

#include <Alegengine/userInterface/UIElement.hpp>
#include <Alegengine/userInterface/DragElement.hpp>
#include <Alegengine/rendering/Window.hpp>
#include <string>

namespace Aleg {
  class ScrollingElement : public UIElement {
  public:
    ScrollingElement(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, float zIndex);
    ScrollingElement(glm::vec2 position, glm::vec2 size, float transparency, const unsigned char* tex, unsigned int len, float zIndex);

    bool horizontalScrolling = false;
    bool verticalScrolling = true;
    float inset = 0.05f;
    float sens = 25.0f;
    std::string layout = "List"; // Options are : List, Grid and None (see docs for more)
  protected:
    void beforeUpdate() override;
    void afterUpdate() override;
  private:
    void initObject();
    void listLayout();
    void scrollCallback(Window* win, double x, double y);

    glm::vec2 scrollAmount = glm::vec2(0.0f, 0.0f);
    float lastY = 0.0f;
    DragElement* horizontalScrollbar;
    DragElement* verticalScrollbar;
  };
}
