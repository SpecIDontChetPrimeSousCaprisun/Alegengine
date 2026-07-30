#pragma once

#include <Alegengine/rendering/Shader.hpp>
#include <Alegengine/objects/DrawInfo.hpp>
#include <Alegengine/objects/CollisionResult.hpp>
#include <Alegengine/objects/CollisionGroup.hpp>
#include <Alegengine/helpers/Logger.hpp>
#include <Alegengine/objects/MaskResult.hpp>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Aleg {
  class Object;
  class Window;

  class RaycastResult {
  public:
    Object* object = nullptr;
    float tHit = 0.0f;
    glm::vec2 hitPoint;
  };

  class Object {
  public:
    static void init();
    static void deletePendingObjects();
    static CollisionResult checkCollision(Object* a, Object* b);
    static RaycastResult* raycast(glm::vec2 origin,
                                  glm::vec2 dir,
                                  CollisionGroup mask = CollisionGroups::Default);

    Object(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, float zIndex, Window* window = nullptr); 
    Object(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, float zIndex, Window* window = nullptr);

    void draw();
    void update();
    void setMask(glm::vec2 maskPos, glm::vec2 maskSize);
    void removeMask();
    virtual void setParent(Object* parent);
    void pendDelete();
    bool isDeleting();
    Object* getParent();
    std::vector<Object*> getChildren();
    MaskResult* getMask();

    glm::vec2 position;
    glm::vec2 realPosition;
    glm::vec2 size;
    glm::vec2 realSize;
    glm::vec2 linearVelocity = glm::vec2(0.0f, 0.0f);

    float zIndex;
    float transparency;
    float angularVelocity = 0.0f;
    float rotation = 0.0f;
    float gravity = 500.0f;
    
    glm::vec3 color;
    glm::vec3 colorChange = glm::vec3(0.0f, 0.0f, 0.0f);
    
    bool usesColor;
    bool visible = true;
    bool anchored = false;
    bool canCollide = true;

    CollisionGroup mask = CollisionGroups::Default;
    CollisionGroup collisionGroup = CollisionGroups::Default;

    Window* window;
  protected:
    virtual ~Object();

    virtual DrawInfo* beforeDrawing();
    virtual void afterDrawing(DrawInfo* info);
    virtual void beforeUpdate();
    virtual void afterUpdate();
    virtual void resolveCollision(Object* object, 
                                  glm::vec2 bestAxis, 
                                  float minOverlap,
                                  glm::vec2 WH,
                                  glm::vec2 WHb);
  private:
    static std::map<float, std::vector<Object*>> objects;
    static Shader* shader;
    static Logger* logger;

    unsigned int VAO, VBO, texture;
    bool pendingDelete = false;
    bool hasMask = false;
    glm::vec2 maskPosition;
    glm::vec2 maskSize;
    Object* parent = nullptr;
    std::vector<Object*> children;
 
    void initObject();
    void removeParent();

    // draw() subfunctions
    void makeModel(DrawInfo* info);
    void sendFragmentInfo(DrawInfo* info);
  };
}
