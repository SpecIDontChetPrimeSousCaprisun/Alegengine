#include <Alegengine/alegengine.hpp>
#include <cmath>
#include <algorithm>

namespace Aleg {
  ScrollingElement::ScrollingElement(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, float zIndex, Window* window)
    : UIElement(position, size, transparency, color, zIndex, window) {initObject();}

  ScrollingElement::ScrollingElement(glm::vec2 position, glm::vec2 size, float transparency, const unsigned char* tex, unsigned int len, float zIndex, Window* window)
    : UIElement(position, size, transparency, tex, len, zIndex, window) {initObject();}

  void ScrollingElement::initObject() {
    Window::scrollCallbacks.push_back([this](Window* win, double x, double y) {scrollCallback(win, x, y);});

    horizontalScrollbar = new DragElement(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), 0.0f, glm::vec3(0.75f, 0.75f, 0.75f), 10000.0f);
    verticalScrollbar = new DragElement(glm::vec2(0.925f, 0.5f), glm::vec2(0.025f, 0.8f), 0.0f, glm::vec3(0.75f, 0.75f, 0.75f), 10000.0f);
  
    horizontalScrollbar->setParent(this);
    horizontalScrollbar->followY = false;
    horizontalScrollbar->anchorPoint = glm::vec2(0.5f, 1.0f);
    horizontalScrollbar->topLeftBounds = glm::vec2(inset, inset);
    horizontalScrollbar->bottomRightBounds = glm::vec2(1.0f - inset, 1.0f - inset);
    verticalScrollbar->setParent(this);
    verticalScrollbar->followX = false;
    verticalScrollbar->anchorPoint = glm::vec2(1.0f, 0.5f);
    verticalScrollbar->topLeftBounds = glm::vec2(inset, inset);
    verticalScrollbar->bottomRightBounds = glm::vec2(1.0f - inset, 1.0f - inset);
  }

  void ScrollingElement::beforeUpdate() {
    horizontalScrollbar->visible = horizontalScrolling;
    verticalScrollbar->visible = verticalScrolling;

    if (layout == "List") listLayout();
  }

  void ScrollingElement::afterUpdate() {}

  void ScrollingElement::listLayout() {
    float y = 0.0f;

    for (Object* object : getChildren()) {
      if (!object->visible) continue;
      if (object == verticalScrollbar || object == horizontalScrollbar) {
        glm::vec2 sizeMask = glm::vec2(realSize.x - (realSize.x * inset), realSize.y - (realSize.y * (inset * 2)));

        object->setMask(realPosition + (realSize * inset), sizeMask);
        continue;
      }

      object->setMask(realPosition, realSize);
      object->position = glm::vec2(inset, y + inset) + scrollAmount; 
      object->size.x = 1.0f - (inset * 2);
      y += object->realSize.y;
    }

    verticalScrollbar->visible = y > realSize.y - (inset * 2) && verticalScrolling;
    verticalScrollbar->size.y = 1 / (y / realSize.y - (inset * 2));

    float expectedScrollPos = ((realSize.y - (inset * 2)) * (-scrollAmount.y / y)) + (verticalScrollbar->size.y / 2) + inset;

    if (y != lastY) verticalScrollbar->position.y = expectedScrollPos;
    else if (verticalScrollbar->position.y != expectedScrollPos && !std::isnan(expectedScrollPos)) {
      scrollAmount.y -= verticalScrollbar->position.y - expectedScrollPos;
    }

    lastY = y;
  }

  void ScrollingElement::scrollCallback(Window* win, double, double y) {
    if (win != window) return;
    if (!verticalScrolling && !horizontalScrolling) return;
    
    recalculateFocus();

    if (focused) {
      if (verticalScrolling) {
        scrollAmount.y += (y / window->fbHeight) * sens;
        float expectedScrollPos = ((realSize.y - (inset * 2)) * (-scrollAmount.y / lastY)) + (verticalScrollbar->size.y / 2) + inset;
        
        verticalScrollbar->position.y = std::clamp(expectedScrollPos, inset, 1.0f - inset);
      } else if (horizontalScrolling) {
        scrollAmount.x += (y / window->fbHeight) * sens;
      }
    }
  }
}
