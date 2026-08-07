#include <Alegengine/userInterface/TextElement.hpp>
#include <Alegengine/rendering/Window.hpp>
#include <shaders/TextVertex.h>
#include <shaders/TextFrag.h>

namespace Aleg {
  std::vector<TextElement*> TextElement::elements;

  void TextElement::init() {
    Window::shaderInfos.push_back(new ShaderInfo("textShader", (const char*)TextVertex_glsl, (const char*)TextFrag_glsl));
  }

  TextElement::TextElement(glm::vec2 position,
                           glm::vec2 size,
                           float transparency,
                           glm::vec3 color,
                           float zIndex,
                           const unsigned char* font,
                           unsigned int fontLen,
                           std::string text,
                           Window* window)
    : UIElement(position, size, transparency, color, zIndex, window), 
      text(text), rawFont(font), fontLen(fontLen) {
    initObject();
  }

  TextElement::TextElement(glm::vec2 position,
                           glm::vec2 size,
                           float transparency,
                           const unsigned char* tex,
                           unsigned int len,
                           float zIndex,
                           const unsigned char* font,
                           unsigned int fontLen,
                           std::string text,
                           Window* window)
    : UIElement(position, size, transparency, tex, len, zIndex, window), 
      text(text), rawFont(font), fontLen(fontLen) {
    initObject();
  }

  void TextElement::initObject() {
    shader = window->shaders["textShader"];
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

    // position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    recalculateFontHeight();
    elements.push_back(this);
  }

  TextElement::~TextElement() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
  }

  void TextElement::afterDrawing(DrawInfo* info) {
    if (!shader) shader = window->shaders["textShader"];
    glUseProgram(shader->program);

    glm::mat4 projection = glm::ortho(
      0.0f, (float)window->fbWidth,
      (float)window->fbHeight, 0.0f,
      -1.0f, 1.0f
    );

    glUniformMatrix4fv(
      glGetUniformLocation(shader->program, "projection"),
      1,
      GL_FALSE,
      glm::value_ptr(projection)
    );

    glUniform3f(
      glGetUniformLocation(shader->program, "color"),
      textColor.r, textColor.g, textColor.b
    );

    glUniform3f(
      glGetUniformLocation(shader->program, "colorChange"),
      0.0f, 0.0f, 0.0f
    );

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font->texture);

    glUniform1i(
      glGetUniformLocation(shader->program, "tex"),
      0
    );

    glUniform1f(
      glGetUniformLocation(shader->program, "transparency"),
      textTransparency
    );

    glUniform1f(
      glGetUniformLocation(shader->program, "rotation"),
      rotation
    );

    glm::vec2 pivot(
      info->position.x + info->size.x * 0.5f,
      info->position.y + info->size.y * 0.5f
    );

    glUniform2f(
      glGetUniformLocation(shader->program, "pivot"),
      pivot.x,
      pivot.y
    );

    // Send mask
    if (info->hasMask) {
      glUniform1i(glGetUniformLocation(shader->program, "hasMask"), 1);
      glUniform2f(
        glGetUniformLocation(shader->program, "maskPos"),
        info->maskPosition.x, info->maskPosition.y
      );

      glUniform2f(
        glGetUniformLocation(shader->program, "maskSize"),
        info->maskSize.x, info->maskSize.y
      );
    } else glUniform1i(glGetUniformLocation(shader->program, "hasMask"), 0);
    
    // Send Object data
    glUniform2f(
      glGetUniformLocation(shader->program, "realPos"),
      info->position.x, info->position.y
    );

    glUniform2f(
      glGetUniformLocation(shader->program, "realSize"),
      info->size.x, info->size.y
    );

    // Send resolution
    glUniform2f(
      glGetUniformLocation(shader->program, "resolution"),
      window->fbWidth, window->fbHeight
    );

    glBindVertexArray(VAO);

    float startX = info->position.x;

    if (textCentered) {
      startX += info->size.x / 2;
      startX -= textWidth / 2;
    }

    float x = startX;
    float y = info->position.y + (info->size.y / 2) + (font->height / 4);

    for (unsigned char c : text) {
      if (c < 32 || c >= 128)
          continue;

      int glyphIndex = c - 32;

      stbtt_aligned_quad q;

      stbtt_GetBakedQuad(
        font->cdata,
        font->BITMAP_W, font->BITMAP_H,
        glyphIndex,
        &x, &y,
        &q,
        1
      );

      float vertices[6][4] = {
        { q.x0, q.y0, q.s0, q.t0 },
        { q.x1, q.y0, q.s1, q.t0 },
        { q.x1, q.y1, q.s1, q.t1 },

        { q.x1, q.y1, q.s1, q.t1 },
        { q.x0, q.y1, q.s0, q.t1 },
        { q.x0, q.y0, q.s0, q.t0 }
      };

      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

      glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    
    textWidth = x - startX;

    glBindVertexArray(0);
  }

  void TextElement::recalculateTextWidth() {
    if (Window::currentWindow != window) return;

    DrawInfo* info = beforeDrawing();

    float startX = info->position.x;
    float x = startX;
    float y = info->position.y + (info->size.y / 2) + (font->height / 4);

    for (unsigned char c : text) {
      if (c < 32 || c >= 128)
          continue;

      int glyphIndex = c - 32;

      stbtt_aligned_quad q;

      stbtt_GetBakedQuad(
        font->cdata,
        font->BITMAP_W, font->BITMAP_H,
        glyphIndex,
        &x, &y,
        &q,
        1
      );
    }
    
    textWidth = x - startX;
  }

  void TextElement::recalculateFontHeight() {
    if (realSize.x < 0.0f) return;
    if (font) delete font;

    DrawInfo* info = beforeDrawing();

    float fontHeight = info->size.y;

    font = new Font(rawFont, fontLen, fontHeight);
    recalculateTextWidth();

    if (textWidth > info->size.x) {
      delete font; 
      font = new Font(rawFont, fontLen, (fontHeight * info->size.x) / textWidth);
    }
  }
}
