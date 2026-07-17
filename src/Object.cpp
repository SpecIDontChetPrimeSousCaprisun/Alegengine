#include "../objects/Object.hpp"
#include "../rendering/Window.hpp"

namespace Aleg {
  std::map<float, std::vector<Object*>> Object::objects;
  Shader* Object::shader;

  void Object::init() {
    shader = new Shader("shaders/Vertex.glsl", "shaders/Frag.glsl");
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

  DrawInfo* Object::beforeDrawing() {
    return new DrawInfo(position, size);
  }

  void Object::afterDrawing() {}

  // draw and its subfunctions
  void Object::drawAll() {
    for (auto& [zIndex, objectVector] : objects) {
      for (Object* object : objectVector) {
        object->draw();
      }
    }
  }

  void Object::draw() {
    if (!visible) return;

    glUseProgram(shader->program);

    DrawInfo* info = beforeDrawing();

    if (!info->shouldDraw) return;

    makeModel(info);
    sendFragmentInfo();

    // Draw
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);

    delete info;
  }

  void Object::makeModel(DrawInfo* info) {
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(
      model,
      glm::vec3(info->position, 0.0f)
    );

    model = glm::translate( // Apply rotation anchor
      model,
      glm::vec3(info->size * 0.5f, 0.0f)
    );

    model = glm::rotate( // Rotate
      model,
      glm::radians(rotation),
      glm::vec3(0.0f, 0.0f, 1.0f)
    );

    model = glm::translate( // Reset back to normal position
      model,
      glm::vec3(-info->size * 0.5f, 0.0f)
    );

    model = glm::scale(
      model,
      glm::vec3(info->size, 1.0f)
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
  }

  void Object::sendFragmentInfo() {
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
  }

  // update
  void Object::updateAll() {
    for (auto& [zIndex, objectVector] : objects) {
      for (Object* object : objectVector) {
        object->update();
      }
    }
  }
 
  void Object::update() {
    if (anchored) return;

    linearVelocity += glm::vec2(0.0f, gravity) * (float)Window::deltaTime;
    position += linearVelocity * (float)Window::deltaTime;
    rotation += angularVelocity * (float)Window::deltaTime;

    if (!canCollide) return;

    for (auto& [zIndex, objectVector] : objects) {
      for (Object* object : objectVector) {
        if (object == this) continue;
        if (!object->canCollide) continue;
        
        CollisionResult result = checkCollision(this, object);

        if (result.hit) {
          glm::vec2 WH(size.x / 2, size.y / 2);
          glm::vec2 WHb(object->size.x / 2, object->size.y / 2);
          resolveCollision(object, result.bestAxis, result.minOverlap, WH, WHb);
        }
      }
    }
  }

  CollisionResult Object::checkCollision(Object* a, Object* b) {
    float aRadians = a->rotation * glm::pi<float>() / 180.0f;
    glm::vec2 Ax(std::cos(aRadians), std::sin(aRadians));
    glm::vec2 Ay(-std::sin(aRadians), std::cos(aRadians));

    float bRadians = b->rotation * glm::pi<float>() / 180.0f;
    glm::vec2 Bx(std::cos(bRadians), std::sin(bRadians));
    glm::vec2 By(-std::sin(bRadians), std::cos(bRadians));

    glm::vec2 WH(a->size.x / 2, a->size.y / 2);
    glm::vec2 WHb(b->size.x / 2, b->size.y / 2);
    glm::vec2 T = (a->position + WH) - (b->position + WHb);

    auto getOverlap = [&](glm::vec2 axis) -> float {
        float ra = WH.x  * std::abs(glm::dot(Ax, axis)) +
                    WH.y  * std::abs(glm::dot(Ay, axis));
        float rb = WHb.x * std::abs(glm::dot(Bx, axis)) +
                    WHb.y * std::abs(glm::dot(By, axis));
        return (ra + rb) - std::abs(glm::dot(T, axis));
    };

    float o1 = getOverlap(Ax);
    float o2 = getOverlap(Ay);
    float o3 = getOverlap(Bx);
    float o4 = getOverlap(By);

    if (o1 <= 0 || o2 <= 0 || o3 <= 0 || o4 <= 0)
        return { false, 0.0f, glm::vec2(0.0f) };

    float minOverlap = o1;
    glm::vec2 bestAxis = Ax;

    if (o2 < minOverlap) { minOverlap = o2; bestAxis = Ay; }
    if (o3 < minOverlap) { minOverlap = o3; bestAxis = Bx; }
    if (o4 < minOverlap) { minOverlap = o4; bestAxis = By; }

    return { true, minOverlap, bestAxis };
  }

  void Object::resolveCollision(Object* object, 
                               glm::vec2 bestAxis, 
                               float minOverlap,
                               glm::vec2 WH,
                               glm::vec2 WHb) {
    glm::vec2 correction = bestAxis * minOverlap;
    glm::vec2 centerA = position + WH;
    glm::vec2 centerB = object->position + WHb;

    glm::vec2 dir = centerA - centerB;

    if (glm::dot(dir, bestAxis) < 0.0f) correction = -correction;

    if (object->anchored) {
      position += correction;
    } else {
      position += correction * 0.5f;
      object->position -= correction * 0.5f;
    }

    float vn = glm::dot(linearVelocity, bestAxis);

    if (vn < 0.0f)
        linearVelocity -= vn * bestAxis;

    float ovn = glm::dot(object->linearVelocity, bestAxis);

    if (ovn < 0.0f)
        object->linearVelocity -= ovn * bestAxis;
  }
}
