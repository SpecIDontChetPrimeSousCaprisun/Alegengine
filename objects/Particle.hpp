#pragma once

#include <Alegengine/objects/Object.hpp>
#include <vector>

namespace Aleg {
  class ParticleInfo {
  public:
    ParticleInfo(glm::vec2 position,
                 glm::vec2 size,
                 float transparency,
                 glm::vec3 color,
                 float zIndex,
                 glm::vec2 dir,
                 float spread,
                 float lifetime,
                 Window* window);

    ParticleInfo(glm::vec2 position,
                 glm::vec2 size,
                 float transparency,
                 const unsigned char* tex,
                 unsigned int len,
                 float zIndex,
                 glm::vec2 dir,
                 float spread,
                 float lifetime,
                 Window* window);

    glm::vec2 position;
    glm::vec2 size;
    float transparency;
    glm::vec3 color;
    const unsigned char* tex;
    unsigned int len;
    float zIndex;
    glm::vec2 dir;
    float spread;
    float lifetime; 
    Window* window;
    bool useColor;
  };

  class Particle : public Object {
  public:
    Particle(glm::vec2 position,
             glm::vec2 size,
             float transparency,
             glm::vec3 color,
             float zIndex,
             glm::vec2 dir,
             float spread,
             float lifetime,
             Window* window = nullptr);

    Particle(glm::vec2 position,
             glm::vec2 size,
             float transparency,
             const unsigned char* tex,
             unsigned int len,
             float zIndex,
             glm::vec2 dir,
             float spread,
             float lifetime,
             Window* window = nullptr);

    static int amntOfParticlesPerFrame;

    static void update();
    static void removeQueuedParticlesWithWindow(Window* window);

    static void spawnParticles(glm::vec2 position,
                               glm::vec2 size,
                               float transparency,
                               glm::vec3 color,
                               float zIndex,
                               glm::vec2 dir,
                               float spread,
                               float lifetime,
                               float quantity,
                               Window* window = nullptr);

    static void spawnParticles(glm::vec2 position,
                               glm::vec2 size,
                               float transparency,
                               const unsigned char* tex,
                               unsigned int len,
                               float zIndex,
                               glm::vec2 dir,
                               float spread,
                               float lifetime,
                               float quantity,
                               Window* window = nullptr);
  protected:
    void beforeUpdate() override;
  private:
    void initObject();

    static void queueParticles(ParticleInfo* info, float quantity);

    static std::vector<ParticleInfo*> particleQueue;

    glm::vec2 dir;
    float spread;
    float lifetime;
  };
}
