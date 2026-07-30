#pragma once

#include <Alegengine/userInterface/UIElement.hpp>
#include <Alegengine/helpers/Font.hpp>

namespace Aleg {
  class TextElement : public UIElement {
  public:
    TextElement(glm::vec2 position,
                glm::vec2 size,
                float transparency,
                glm::vec3 color,
                float zIndex,
                std::string fontPath,
                std::string text);

    TextElement(glm::vec2 position,
                glm::vec2 size,
                float transparency,
                std::string texPath,
                float zIndex,
                std::string fontPath,
                std::string text);

    static void init();

    void recalculateTextWidth();
    void recalculateFontHeight();

    std::string text;

    glm::vec3 textColor = glm::vec3(1.0f, 1.0f, 1.0f);

    float textWidth = 0.0f;
    float textTransparency = 0.0f;

    bool textCentered = true;
  protected:
    virtual ~TextElement() override;

    //DrawInfo* beforeDrawing() override;
    void afterDrawing(DrawInfo* info) override;
  private:
    static Shader* shader;
    static std::vector<TextElement*> elements;

    void initObject();

    unsigned int VAO, VBO;
    std::string fontPath;
    Font* font = nullptr;
  };
}
