#include <Alegengine/objects/ParentObject.hpp>
#include <Alegengine/userInterface/TextElement.hpp>

namespace Aleg {
  Logger* ParentObject::logger = new Logger("ParentObject");

  ParentObject::ParentObject() 
    : Object(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 0.0f) {
    anchored = true;
  }

  void ParentObject::setParent(Object*) {
    logger->warn("Attempting to set parent of parent object");
  }

  void ParentObject::recursivelyRecalculateFonts() {
    performRecursiveOperation("font", this);
  }

  void ParentObject::recursivelyDrawChildren() {
    performRecursiveOperation("draw", this);
  }

  void ParentObject::recursivelyUpdateChildren() {
    performRecursiveOperation("update", this);
  }

  void ParentObject::recursivelyDeleteChildren() {
    performRecursiveOperation("delete", this);
  }

  std::vector<Object*> ParentObject::getChildrenInZOrder(Object* obj) {
    std::vector<Object*> objs;
    std::map<float, std::vector<Object*>> sortedObjects;

    for (Object* loopedObj : obj->getChildren()) {
      sortedObjects[loopedObj->zIndex].push_back(loopedObj);
    }

    for (auto& [zIndex, objVector] : sortedObjects) {
      for (Object* loopedObj : objVector) {
        objs.push_back(loopedObj);
      }
    }

    return objs;
  }

  void ParentObject::performRecursiveOperation(std::string operation, Object* obj) {
    for (Object* loopedObj : getChildrenInZOrder(obj)) {
      if (operation == "update") loopedObj->update();
      else if (operation == "draw") loopedObj->draw();
      else if (operation == "delete") loopedObj->pendDelete();
      else if (operation == "font") {
        TextElement* txt = dynamic_cast<TextElement*>(loopedObj);

        if (!txt) continue;

        txt->recalculateFontHeight();
      }
    }

    for (Object* loopedObj : getChildrenInZOrder(obj)) {
      performRecursiveOperation(operation, loopedObj);
    }
  }
}
