# Alegengine docs
## Shader

Shader is a helper class for glad's rendering
```cpp
Aleg::Shader* shader = new Aleg::Shader("path/to/vertex/shader.glsl", "path/to/fragment/shader.glsl");
glUniform1f( // Send parameters to shaders like this
  glGetUniformLocation(shader->program, "alpha"),
  1 - transparency
);
```

It is recommended to use shaders globaly instead of creating one per object :
```cpp
// CustomObject.hpp
#pragma once

#include <Alegengine/alegengine.hpp>

class CustomObject : public Aleg::Object {
private:
  static Shader* shader; // Use this to draw
};
```
