# Alegengine docs
## Object

Objects are the fondation for your games. You can create them with these two constructors :
```
Aleg::Object(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, float zIndex);
Aleg::Object(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, float zIndex);
```

Along these constructors, you can further customise them with :
```
Aleg::Object->linearVelocity; // glm::vec2 (be careful, this is where gravity is applied in side mode)
Aleg::Object->angularVelocity; // float
Aleg::Object->rotation; // float
Aleg::Object->gravity; // float (defaults to 500.0f)
Aleg::Object->visible; // bool
Aleg::Object->anchored; // bool (when set to true : stops all physics updates for that object)
Aleg::Object->canCollide; // bool
Aleg::Object->mask; // Aleg::CollisionGroups
Aleg::Object->collisionGroup; // Aleg::CollisionGroups
```

If you need to add custom shader, or make the object do something before / after rendering / updating you can just inherit from it.
```
// CustomObject.hpp
#pragma once

#include <Alegengine/alegengine.hpp>

class CustomObject : public Aleg::Object {
public:
  CustomObject(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, float zIndex); 
  CustomObject(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, float zIndex);
protected:
  ~Object() override;

  DrawInfo* beforeDrawing() override;
  void afterDrawing() override;
  void resolveCollision(Object* object, 
                        glm::vec2 bestAxis, 
                        float minOverlap,
                        glm::vec2 WH,
                        glm::vec2 WHb) override;
};
```
