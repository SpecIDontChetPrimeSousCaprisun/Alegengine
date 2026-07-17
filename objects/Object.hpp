#pragma once

#include "../rendering/Shader.hpp"

#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Aleg {
  class Object {
  public:
    Object(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, float zIndex);

    static void init();
    static void drawAll();
    static void updateAll();

    glm::vec2 position;
    glm::vec2 size;
    glm::vec2 rotateAnchor = glm::vec2(0.5f, 0.5f);
    glm::vec3 color;
    float transparency;
    float rotation = 0.0f;
    bool usesColor;
    bool visible = true;
  protected:
  private:
    static std::map<float, std::vector<Object*>> objects;
    static Shader* shader;

    unsigned int VAO, VBO;
    float zIndex;

    void draw();
    void update();
    void initObject();
  };
}
