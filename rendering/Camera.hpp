#pragma once

#include <Alegengine/objects/Object.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Aleg {
  class Camera : public Object {
  public:
    static Camera* currentCamera;

    Camera(glm::vec2 position);
  protected:
    DrawInfo* beforeDrawing() override;
    void beforeUpdate() override;
  };
}
