#pragma once

#include "../rendering/Shader.hpp"
#include "DrawInfo.hpp"
#include "CollisionResult.hpp"
#include "CollisionGroup.hpp"

#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Aleg {
  class Object;

  class RaycastResult {
  public:
    Object* object = nullptr;
    float tHit = 0.0f;
    glm::vec2 hitPoint;
  };

  class Object {
  public:
    static void init();
    static void drawAll();
    static void updateAll();
    static CollisionResult checkCollision(Object* a, Object* b);
    static RaycastResult* raycast(glm::vec2 origin,
                                  glm::vec2 dir,
                                  CollisionGroup mask = CollisionGroups::Default);

    Object(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, float zIndex); 
    Object(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, float zIndex);

    void pendDelete();
    bool isDeleting();

    glm::vec2 position;
    glm::vec2 realPosition;
    glm::vec2 size;
    glm::vec2 realSize;
    glm::vec2 linearVelocity = glm::vec2(0.0f, 0.0f);

    float transparency;
    float angularVelocity = 0.0f;
    float rotation = 0.0f;
    float gravity = 500.0f;
    
    glm::vec3 color;
    
    bool usesColor;
    bool visible = true;
    bool anchored = false;
    bool canCollide = true;

    Object* parent = nullptr;

    CollisionGroup mask = CollisionGroups::Default;
    CollisionGroup collisionGroup = CollisionGroups::Default;
  protected:
    virtual ~Object();

    virtual DrawInfo* beforeDrawing();
    virtual void afterDrawing();
    virtual void beforeUpdate();
    virtual void afterUpdate();
    virtual void resolveCollision(Object* object, 
                                  glm::vec2 bestAxis, 
                                  float minOverlap,
                                  glm::vec2 WH,
                                  glm::vec2 WHb);
  private:
    static void deletePendingObjects();

    static std::map<float, std::vector<Object*>> objects;
    static Shader* shader;

    unsigned int VAO, VBO, texture;
    float zIndex;
    bool pendingDelete = false;

    void draw();
    void update();
    void initObject();

    // draw() subfunctions
    void makeModel(DrawInfo* info);
    void sendFragmentInfo();
  };
}
