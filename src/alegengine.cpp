#include <Alegengine/alegengine.hpp>

#ifdef _WIN32
#include <windows.h>

void enableAnsiOnWindows() {
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD mode = 0;
  GetConsoleMode(hOut, &mode);
  SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
#endif

namespace Aleg {
  std::string type;

  void init(std::string type) {
    #ifdef _WIN32
      enableAnsiOnWindows();
    #endif

    Aleg::type = type;

    Window::init();
    Object::init();
    Camera::currentCamera = new Camera(glm::vec2(0.0f, 0.0f));
  }

  void mainLoop() {
    Window::mainLoop();
  }
}
