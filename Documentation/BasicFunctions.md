# Alegengine docs
## Basic functions

### init()
Before doing anything with this library you have to call :
```cpp
Aleg::init()
```

It takes one argument :
std::string type 
Specifies the type of the game, can be
+ "topdown" default
+ "side"

### mainLoop()
Calls the main loop and starts rendering the game.
Yields until the game window is closed.
