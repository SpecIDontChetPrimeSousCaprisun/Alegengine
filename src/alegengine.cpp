#include "../alegengine.hpp"

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
  }

  void mainLoop() {
    Window::mainLoop();
  }
}
