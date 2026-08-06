#include <Alegengine/alegengine.hpp>
#include <cmath>
#include <sstream>
#include <shaders/Vertex.h>
#include <shaders/Frag.h>

namespace Aleg {
  std::map<float, std::vector<Object*>> Object::objects;
  Logger* Object::logger = new Logger("Object");

  void Object::init() {
    Window::shaderInfos.push_back(new ShaderInfo("objShader", (const char*)Vertex_glsl, (const char*) Frag_glsl));
  }

  Object::Object(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, float zIndex, Window* window) 
    : position(position), size(size), zIndex(zIndex), transparency(transparency), color(color), usesColor(true), window(window) {
    initObject();
  }

  Object::Object(glm::vec2 position, glm::vec2 size, float transparency, const unsigned char* tex, unsigned int len, float zIndex, Window* window) 
    : position(position), size(size), zIndex(zIndex), transparency(transparency), usesColor(false), window(window), texture(FileLoader::loadTexture(tex, len, window)) {
    initObject();
  }

  void Object::initObject() {
    if (!window) window = Window::currentWindow;
    glfwMakeContextCurrent(window->window);
    shader = window->shaders["objShader"];

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

    setParent(window->parent);

    objects[zIndex].push_back(this);
  }

  // draw
  DrawInfo* Object::beforeDrawing() {
    DrawInfo* info = new DrawInfo(realPosition, realSize); 

    if (window->cam) info->position -= (window->cam->realPosition / parallaxFactor);
    if (hasMask) {
      info->hasMask = true;
      info->maskPosition = maskPosition;
      info->maskSize = maskSize;
    }

    return info;
  }

  void Object::afterDrawing(DrawInfo*) {}

  void Object::draw() {
    if (!visible) return;
    if (!shader) shader = window->shaders["objShader"];
    if (!shader) return;

    glfwMakeContextCurrent(window->window);
    glUseProgram(shader->program);

    DrawInfo* info = beforeDrawing();

    if (!info->shouldDraw) return;

    makeModel(info);
    sendFragmentInfo(info);

    // Draw
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    afterDrawing(info);

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
      (float)window->fbWidth,
      (float)window->fbHeight,
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

  void Object::sendFragmentInfo(DrawInfo* info) {
    // Send color stuff
    glUniform1f(
      glGetUniformLocation(shader->program, "alpha"),
      1 - transparency
    );

    glUniform1i(
      glGetUniformLocation(shader->program, "useColor"),
      usesColor ? 1 : 0
    ); 

    glUniform3f(
      glGetUniformLocation(shader->program, "colorChange"),
      colorChange.x, colorChange.y, colorChange.z
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

    if (usesColor) { // Send color / texture
      glUniform3f(
        glGetUniformLocation(shader->program, "color"),
        color.x, color.y, color.z
      );
    } else {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture);

      glUniform1i(
        glGetUniformLocation(shader->program, "tex"),
        0
      );
    }
  }

  // update
  void Object::beforeUpdate() {}
  void Object::afterUpdate() {}

  void Object::update() {
    beforeUpdate();

    glm::vec2 realAnchorPoint = glm::vec2(anchorPoint.x, -anchorPoint.y);

    if (parent) realPosition = (parent->realPosition + position) + (size * realAnchorPoint);
    else realPosition = position + (size * realAnchorPoint);

    realSize = size;

    if (anchored) {
      afterUpdate();
      return;
    }

    if (type == "side") linearVelocity += glm::vec2(0.0f, gravity) * (float)window->deltaTime;
    position += linearVelocity * (float)window->deltaTime;
    rotation += angularVelocity * (float)window->deltaTime;

    if (parent) realPosition = (parent->realPosition + position) + (size * realAnchorPoint);
    else realPosition = position + (size * realAnchorPoint);

    if (!canCollide) {
      afterUpdate();
      return;
    }

    for (auto& [zIndex, objectVector] : objects) {
      for (Object* object : objectVector) {
        if (object == this) continue;
        if (object->window != window) continue;
        if (!object->canCollide) continue;
        if (object->collisionGroup != mask) continue;

        CollisionResult result = checkCollision(this, object);

        if (result.hit) {
          glm::vec2 WH(realSize.x / 2, realSize.y / 2);
          glm::vec2 WHb(object->realSize.x / 2, object->realSize.y / 2);
          resolveCollision(object, result.bestAxis, result.minOverlap, WH, WHb);
        }
      }
    }

    if (parent) position = (realPosition - parent->realPosition) + (size * realAnchorPoint);
    else position = realPosition + (size * realAnchorPoint);

    afterUpdate();
  }

  CollisionResult Object::checkCollision(Object* a, Object* b) {
    float aRadians = a->rotation * glm::pi<float>() / 180.0f;
    glm::vec2 Ax(std::cos(aRadians), std::sin(aRadians));
    glm::vec2 Ay(-std::sin(aRadians), std::cos(aRadians));

    float bRadians = b->rotation * glm::pi<float>() / 180.0f;
    glm::vec2 Bx(std::cos(bRadians), std::sin(bRadians));
    glm::vec2 By(-std::sin(bRadians), std::cos(bRadians));

    glm::vec2 WH(a->realSize.x / 2, a->realSize.y / 2);
    glm::vec2 WHb(b->realSize.x / 2, b->realSize.y / 2);
    glm::vec2 T = (a->realPosition + WH) - (b->realPosition + WHb);

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
    glm::vec2 centerA = realPosition + WH;
    glm::vec2 centerB = object->realPosition + WHb;

    glm::vec2 dir = centerA - centerB;

    if (glm::dot(dir, bestAxis) < 0.0f) correction = -correction;

    if (object->anchored) {
      realPosition += correction;
    } else {
      realPosition += correction * 0.5f;
      object->realPosition -= correction * 0.5f;
    } 

    float vn = glm::dot(linearVelocity, bestAxis);

    if (vn < 0.0f)
        linearVelocity -= vn * bestAxis;

    float ovn = glm::dot(object->linearVelocity, bestAxis);

    if (ovn < 0.0f)
        object->linearVelocity -= ovn * bestAxis;

    if (correction.y < 0.0f && object->rotation == 0.0f) {
      linearVelocity.y = 0.0f;
    }
  }

  // delete
  void Object::deletePendingObjects() {
    for (auto& [zIndex, objectsVector] : objects) {
      for (auto it = objectsVector.begin(); it != objectsVector.end(); ) {
        Object* object = *it;

        if (object->pendingDelete) {
          it = objectsVector.erase(it);
          delete object;
        }
        else {
          ++it;
        }
      }
    }
  }

  Object::~Object() {
    removeParent(); 
    glfwMakeContextCurrent(window->window);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    if (!usesColor) {
      glDeleteTextures(1, &texture);
    }
  }

  void Object::pendDelete() {
    pendingDelete = true;
  }

  bool Object::isDeleting() {
    return pendingDelete;
  }

  // raycast
  RaycastResult* Object::raycast(glm::vec2 origin,
                                 glm::vec2 dir,
                                 Window* window,
                                 CollisionGroup mask) {
    if (!window) window = Window::currentWindow;

    Object* closestObj = nullptr;
    float maxDist = glm::length(dir);
    float closestT = maxDist;

    glm::vec2 dirNorm = glm::normalize(dir);

    for (auto& [zIndex, objectsVector] : objects) {
      for (Object* object : objectsVector) {
        if (!object->canCollide) continue;
        if (object->collisionGroup != mask) continue;
        if (object->window != window) continue;

        float rad = object->rotation * glm::pi<float>() / 180.0f;

        glm::vec2 Ax(std::cos(rad), std::sin(rad));
        glm::vec2 Ay(-std::sin(rad), std::cos(rad));

        // half extents
        glm::vec2 half = object->realSize * 0.5f;

        // center of box
        glm::vec2 center = object->realPosition + half;

        // Transform ray into OBB space
        glm::vec2 relOrigin = origin - center;

        glm::vec2 localOrigin(
            glm::dot(relOrigin, Ax),
            glm::dot(relOrigin, Ay)
        );

        glm::vec2 localDir(
            glm::dot(dirNorm, Ax),
            glm::dot(dirNorm, Ay)
        );

        // AABB slab test in local space
        glm::vec2 min(-half.x, -half.y);
        glm::vec2 max( half.x,  half.y);

        float tMin = 0.0f;
        float tMax = maxDist;

        for (int i = 0; i < 2; i++) {
          float o = (i == 0) ? localOrigin.x : localOrigin.y;
          float d = (i == 0) ? localDir.x : localDir.y;
          float mn = (i == 0) ? min.x : min.y;
          float mx = (i == 0) ? max.x : max.y;

          if (fabs(d) < 1e-8f) {
            if (o < mn || o > mx) {
              tMin = 2.0f;
              break;
            }
          } else {
            float invD = 1.0f / d;
            float t1 = (mn - o) * invD;
            float t2 = (mx - o) * invD;

            if (t1 > t2) std::swap(t1, t2);

            tMin = std::max(tMin, t1);
            tMax = std::min(tMax, t2);

            if (tMin > tMax) break;
          }
        }

        if (tMin > tMax) continue;
        if (tMax < 0.0f) continue;

        float t = std::max(tMin, 0.0f);

        if (t < closestT) {
          closestT = t;
          closestObj = object;
        }
      }
    }
  
    if (closestObj) {
      RaycastResult* result = new RaycastResult();

      result->tHit = closestT;
      result->hitPoint = origin + dirNorm * closestT;
      result->object = closestObj;
      return result;
    }

    return nullptr;
  }

  // Object in bounds
  std::vector<Object*> Object::getObjectsInBounds(glm::vec2 position,
                                                  glm::vec2 size,
                                                  float rotation,
                                                  Window* window,
                                                  CollisionGroup mask) {
    if (!window) window = Window::currentWindow;

    Object* tempObj = new Object(position, size, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f), 10000000.0f, window);
    tempObj->rotation = rotation;
    tempObj->anchored = true;
    tempObj->canCollide = false;
    tempObj->collisionGroup = CollisionGroups::Temp;
    tempObj->update();

    std::vector<Object*> result;

    for (auto& [zIndex, objectVector] : objects) {
      for (Object* obj : objectVector) {
        if (obj == tempObj) continue;
        if (obj->window != tempObj->window) continue;
        if (obj->collisionGroup != mask) continue;

        if (checkCollision(tempObj, obj).hit) {
          result.push_back(obj);
        }
      }
    }

    return result;
  }

  // parenting
  void Object::setParent(Object* parent) {
    removeParent();

    if (!parent) return;
    if (parent == this) return;

    this->parent = parent;
    this->parent->children.push_back(this);
  }

  void Object::removeParent() {
    if (this->parent) {
      for (auto it = this->parent->children.begin(); it != this->parent->children.end(); ) {
        if (*it == this) {
          it = this->parent->children.erase(it);
          break;
        } else {
          ++it;
        }
      }

      this->parent = nullptr;
    }
  }

  Object* Object::getParent() {
    return parent;
  }

  std::vector<Object*> Object::getChildren() {
    return children;
  }

  // mask
  void Object::setMask(glm::vec2 maskPos, glm::vec2 maskSize) {
    hasMask = true;
    maskPosition = maskPos;
    this->maskSize = maskSize;
  }

  void Object::removeMask() {
    hasMask = false;
  }

  MaskResult* Object::getMask() {
    MaskResult* result = new MaskResult();

    result->hasMask = hasMask;
    result->position = maskPosition;
    result->size = maskSize;

    return result;
  }
}
