# Alegengine

Alegengine (short for Alegation-engine) is a 2D C++ game engine / library.
This engine supports : Linux and Windows.
Web support will come one day.
There may be macOS support eventually.

DON'T USE THIS FOR ANY SERIOUS USE
USE [RAYLIB](https://www.raylib.com/) INSTEAD

## AI disclosure

AI may have been used for generating small parts of the engine.
Old AI generated code may have Integrated copied over to this repository.
This project is majorly vibe-free.

## How to link to the project

### 1) Add it to your include path
```
cd include # Go into your include path
git clone https://github.com/SpecIDontChetPrimeSousCaprisun/Alegengine.git
```

### 2) Build your project
Basic g++ example of a build command :

```
g++ -Wall -Wextra \
  -Iinclude include/Alegengine/src/* \
  -Iinclude/Alegengine/third-party main.cpp \
  -o game \
  -lglfw \
  -lGL \
  -ldl \
  -lpthread \
  -lX11 \
  -lXrandr \
  -lXi \
  -lXxf86vm \
  -lXcursor \
  -lm
```

Go see the make file in [Alegengine test](https://github.com/SpecIDontChetPrimeSousCaprisun/AlegengineTest/tree/main) for a bigger implementation.

## What is possible with this engine ?
I am currently working on the engine itself.
When it will be done i will make a side view platformer game and a topdown game to showcase what this engine is really capable of.

## Where can i learn it ?
[RTFM](Documentation/README.md)
