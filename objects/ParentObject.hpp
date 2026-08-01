#pragma once

#include <Alegengine/objects/Object.hpp>
#include <vector>

namespace Aleg {
  class ParentObject : public Object {
  public:
    static Logger* logger;

    ParentObject();

    void setParent(Object* parent) override;
    void recursivelyUpdateChildren();
    void recursivelyDrawChildren();
    void recursivelyRecalculateFonts();
    void recursivelyDeleteChildren();
  private:
    static std::vector<Object*> getChildrenInZOrder(Object* obj);
    static void performRecursiveOperation(std::string operation, Object* obj);
  };
}
