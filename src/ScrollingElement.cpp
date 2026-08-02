#include <Alegengine/alegengine.hpp>

namespace Aleg {
  ScrollingElement::ScrollingElement(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, float zIndex)
    : UIElement(position, size, transparency, color, zIndex) {initObject();}

  ScrollingElement::ScrollingElement(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, float zIndex)
    : UIElement(position, size, transparency, texPath, zIndex) {initObject();}

  void ScrollingElement::initObject() {
    Window::scrollCallbacks.push_back([this](Window* win, double x, double y) {scrollCallback(win, x, y);});

    horizontalScrollbar = new UIElement(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), 0.0f, glm::vec3(0.75f, 0.75f, 0.75f), 10000.0f);
    verticalScrollbar = new UIElement(glm::vec2(0.9f, 0.1f), glm::vec2(0.05f, 0.8f), 0.0f, glm::vec3(0.75f, 0.75f, 0.75f), 10000.0f);
  
    horizontalScrollbar->setParent(this);
    verticalScrollbar->setParent(this);
  }

  void ScrollingElement::beforeUpdate() {
    if (layout == "List") listLayout();
  }

  void ScrollingElement::afterUpdate() {
    horizontalScrollbar->visible = horizontalScrolling;
    verticalScrollbar->visible = verticalScrolling;
  }

  void ScrollingElement::listLayout() {
    float y = 0.0f;

    for (Object* object : getChildren()) {
      if (object == verticalScrollbar || object == horizontalScrollbar) continue;
      if (!object->visible) continue;

      object->position = glm::vec2(inset, y + inset) + scrollAmount;
      object->setMask(realPosition, realSize);
      object->size.x = realSize.x - (inset * 2);
      y += object->size.y;
    }
  }

  void ScrollingElement::scrollCallback(Window* win, double, double y) {
    if (win != window) return;
    if (!verticalScrolling && !horizontalScrolling) return;
    
    recalculateFocus();

    if (focused) {
      if (verticalScrolling) {
        scrollAmount.y += (y / window->fbHeight) * sens;
      } else if (horizontalScrolling) {
        scrollAmount.x += (y / window->fbHeight) * sens;
      }
    }
  }
}
