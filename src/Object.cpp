#include "../objects/Object.hpp"
#include "../rendering/Window.hpp"

namespace Aleg {
  std::map<float, std::vector<Object*>> Object::objects;
  Shader* Object::shader;

  void Object::init() {
    shader = new Shader("shaders/Vertex.glsl", "shaders/Frag.glsl");
  }

  void Object::drawAll() {
    for (auto& [zIndex, objectVector] : objects) {
      for (Object* object : objectVector) {
        object->draw();
      }
    }
  }

  Object::Object(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, float zIndex) 
    : position(position), size(size), transparency(transparency), color(color), usesColor(true), zIndex(zIndex) {
    initObject();
  }

  void Object::initObject() {
    float vertices[] = {
      // positions   // UVs
      0.0f, 0.0f,    0.0f, 0.0f,
      1.0f, 0.0f,    1.0f, 0.0f,
      1.0f, 1.0f,    1.0f, 1.0f,

      1.0f, 1.0f,    1.0f, 1.0f,
      0.0f, 1.0f,    0.0f, 1.0f,
      0.0f, 0.0f,    0.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    // position
    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        4 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    // UV
    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        4 * sizeof(float),
        (void*)(2 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    objects[zIndex].push_back(this);
  }

  void Object::draw() {
    if (!visible) return;

    glUseProgram(shader->program);

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(
      model,
      glm::vec3(position, 0.0f)
    );

    model = glm::translate( // Apply rotation anchor
      model,
      glm::vec3(size * rotateAnchor, 0.0f)
    );

    model = glm::rotate( // Rotate
      model,
      rotation,
      glm::vec3(0.0f, 0.0f, 1.0f)
    );

    model = glm::translate( // Reset back to normal size
      model,
      glm::vec3(-size * rotateAnchor, 0.0f)
    );

    model = glm::scale(
      model,
      glm::vec3(size, 1.0f)
    );

    // Orthographic projection
    glm::mat4 projection = glm::ortho(
      0.0f,
      (float)Window::fbWidth,
      (float)Window::fbHeight,
      0.0f,
      -1.0f,
      1.0f
    );

    // Send matrices
    glUniformMatrix4fv(
      glGetUniformLocation(shader->program, "model"),
      1,
      GL_FALSE,
      glm::value_ptr(model)
    );

    glUniformMatrix4fv(
      glGetUniformLocation(shader->program, "projection"),
      1,
      GL_FALSE,
      glm::value_ptr(projection)
    );

    // Send fragment shader stuff
    glUniform1f(
      glGetUniformLocation(shader->program, "alpha"),
      1 - transparency
    );

    glUniform1i(
      glGetUniformLocation(shader->program, "useColor"),
      usesColor ? 1 : 0
    );

    glUniform3f(
      glGetUniformLocation(shader->program, "color"),
      color.x, color.y, color.z
    );

    // Draw
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
  }
}
