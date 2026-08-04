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
                const unsigned char* font,
                unsigned int fontLen,
                std::string text,
                Window* window = nullptr);

    TextElement(glm::vec2 position,
                glm::vec2 size,
                float transparency,
                const unsigned char* tex,
                unsigned int len,
                float zIndex,
                const unsigned char* font,
                unsigned int fontLen,
                std::string text,
                Window* window = nullptr);

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
    static std::vector<TextElement*> elements;

    void initObject();

    unsigned int VAO, VBO;
    const unsigned char* rawFont;
    unsigned int fontLen;
    Font* font = nullptr;
    Shader* shader = nullptr;
  };
}
