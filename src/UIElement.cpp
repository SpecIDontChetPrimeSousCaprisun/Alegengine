#include <Alegengine/alegengine.hpp>

namespace Aleg {
  UIElement::UIElement(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, float zIndex, Window* window) 
    : Object(position, size, transparency, color, zIndex + 999, window) {
    initObject();
  }

  UIElement::UIElement(glm::vec2 position, glm::vec2 size, float transparency, const unsigned char* tex, unsigned int len, float zIndex, Window* window) 
    : Object(position, size, transparency, tex, len, zIndex + 999, window) {
    initObject();
  }

  void UIElement::initObject() {
    anchored = true;
    canCollide = false;
  }

  DrawInfo* UIElement::beforeDrawing() {
    DrawInfo* info = new DrawInfo(realPosition * glm::vec2(window->fbWidth, window->fbHeight), realSize * glm::vec2(window->fbWidth, window->fbHeight));
    MaskResult* maskInfo = getMask();

    info->hasMask = maskInfo->hasMask;
    if (maskInfo->hasMask) {
      info->maskPosition = maskInfo->position * glm::vec2(window->fbWidth, window->fbHeight);
      info->maskSize = maskInfo->size * glm::vec2(window->fbWidth, window->fbHeight);
    }

    return info;
  }

  void UIElement::beforeUpdate() {
    recalculateFocus();
  }
  
  void UIElement::afterUpdate() {
    if (!getParent()) return;

    ParentObject* parent = dynamic_cast<ParentObject*>(getParent());

    if (parent) return;

    glm::vec2 realAnchorPoint = glm::vec2(anchorPoint.x, -anchorPoint.y);

    realSize = getParent()->realSize * realSize;
    realPosition = (getParent()->realPosition + (getParent()->realSize * position)) + (realSize * realAnchorPoint);
  }

  // Focus
  bool UIElement::getFocus() {
    return focused;
  }

  void UIElement::recalculateFocus() {
    double mouseX;
    double mouseY;

    glfwGetCursorPos(window->window, &mouseX, &mouseY);

    DrawInfo* info = beforeDrawing();

    focused = info->position.x < mouseX &&
              info->position.x + info->size.x > mouseX &&
              info->position.y < mouseY &&
              info->position.y + info->size.y > mouseY;
  }
}
