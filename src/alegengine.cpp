#include "../alegengine.hpp"

namespace Aleg {
  void init(std::string type) {
    Window::init();
    Object::init();
  }

  void mainLoop() {
    Window::mainLoop();
  }
}
