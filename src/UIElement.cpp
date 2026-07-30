#include <Alegengine/alegengine.hpp>

namespace Aleg {
  UIElement::UIElement(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, float zIndex) 
    : Object(position, size, transparency, color, zIndex + 999) {
    initObject();
  }

  UIElement::UIElement(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, float zIndex) 
    : Object(position, size, transparency, texPath, zIndex + 999) {
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
}
