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
  void init(std::string type) {
    #ifdef _WIN32
      enableAnsiOnWindows();
    #endif

    Window::init();
    Object::init();
  }

  void mainLoop() {
    Window::mainLoop();
  }
}
