#include <Alegengine/alegengine.hpp>

namespace Aleg {
  ScrollingElement::ScrollingElement(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, float zIndex)
    : UIElement(position, size, transparency, color, zIndex) {initObject();}

  ScrollingElement::ScrollingElement(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, float zIndex)
    : UIElement(position, size, transparency, texPath, zIndex) {initObject();}

  void ScrollingElement::initObject() {
    Window::scrollCallbacks.push_back([this](Window* win, double x, double y) {scrollCallback(win, x, y);});
  }

  void ScrollingElement::beforeUpdate() {
    if (layout == "List") listLayout();
  }

  void ScrollingElement::listLayout() {
    float y = 0.0f;

    for (Object* object : getChildren()) {
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
