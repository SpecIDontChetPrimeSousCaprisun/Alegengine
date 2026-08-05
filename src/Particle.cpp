#include <Alegengine/alegengine.hpp>

namespace Aleg {
  int Particle::amntOfParticlesPerFrame = 5;
  std::vector<ParticleInfo*> Particle::particleQueue;

  Particle::Particle(glm::vec2 position,
                     glm::vec2 size,
                     float transparency,
                     glm::vec3 color,
                     float zIndex,
                     glm::vec2 dir,
                     float spread,
                     float lifetime,
                     Window* window) 
    : Object(position, size, transparency, color, zIndex, window), dir(dir), spread(spread), lifetime(lifetime) {
    initObject();
  }

  Particle::Particle(glm::vec2 position,
                     glm::vec2 size,
                     float transparency,
                     const unsigned char* tex,
                     unsigned int len,
                     float zIndex,
                     glm::vec2 dir,
                     float spread,
                     float lifetime,
                     Window* window) 
    : Object(position, size, transparency, tex, len, zIndex, window), dir(dir), spread(spread), lifetime(lifetime) {
    initObject();
  }

  void Particle::beforeUpdate() {
    lifetime -= window->deltaTime;

    if (lifetime <= 0.0f) pendDelete();
  }

  ParticleInfo::ParticleInfo(glm::vec2 position,
                             glm::vec2 size,
                             float transparency,
                             glm::vec3 color,
                             float zIndex,
                             glm::vec2 dir,
                             float spread,
                             float lifetime,
                             Window* window) 
    : position(position), 
      size(size), 
      transparency(transparency),
      color(color),
      zIndex(zIndex),
      dir(dir),
      spread(spread),
      lifetime(lifetime),
      window(window),
      useColor(true) {}

  ParticleInfo::ParticleInfo(glm::vec2 position,
                             glm::vec2 size,
                             float transparency,
                             const unsigned char* tex,
                             unsigned int len,
                             float zIndex,
                             glm::vec2 dir,
                             float spread,
                             float lifetime,
                             Window* window)
    : position(position), 
      size(size), 
      transparency(transparency),
      tex(tex),
      len(len),
      zIndex(zIndex),
      dir(dir),
      spread(spread),
      lifetime(lifetime),
      window(window),
      useColor(false) {}

  void Particle::initObject() {
    anchored = false;
    canCollide = false;
    collisionGroup = CollisionGroups::Particle;

    std::uniform_real_distribution<float> dist(-spread, spread);

    linearVelocity = dir + glm::vec2(dist(rng), dist(rng));
  }

  void Particle::spawnParticles(glm::vec2 position,
                                glm::vec2 size,
                                float transparency,
                                glm::vec3 color,
                                float zIndex,
                                glm::vec2 dir,
                                float spread,
                                float lifetime,
                                float quantity,
                                Window* window) {
    queueParticles(new ParticleInfo(position,
                                    size,
                                    transparency,
                                    color,
                                    zIndex,
                                    dir,
                                    spread,
                                    lifetime,
                                    window), quantity);
  }

  void Particle::spawnParticles(glm::vec2 position,
                                glm::vec2 size,
                                float transparency,
                                const unsigned char* tex,
                                unsigned int len,
                                float zIndex,
                                glm::vec2 dir,
                                float spread,
                                float lifetime,
                                float quantity,
                                Window* window) {
    queueParticles(new ParticleInfo(position,
                                    size,
                                    transparency,
                                    tex,
                                    len,
                                    zIndex,
                                    dir,
                                    spread,
                                    lifetime,
                                    window), quantity);
  }

  void Particle::queueParticles(ParticleInfo* info, float quantity) {
    for (int i = 0; i < quantity; i++) {
      particleQueue.push_back(info);
    }
  }

  void Particle::update() {
    for (int i = 0; i < amntOfParticlesPerFrame; i++) {
      if (particleQueue.empty()) return;

      if (particleQueue[0]->useColor) {
        new Particle(particleQueue[0]->position,
                    particleQueue[0]->size,
                    particleQueue[0]->transparency,
                    particleQueue[0]->color,
                    particleQueue[0]->zIndex,
                    particleQueue[0]->dir,
                    particleQueue[0]->spread,
                    particleQueue[0]->lifetime,
                    particleQueue[0]->window);
      } else {
        new Particle(particleQueue[0]->position,
                    particleQueue[0]->size,
                    particleQueue[0]->transparency,
                    particleQueue[0]->tex,
                    particleQueue[0]->len,
                    particleQueue[0]->zIndex,
                    particleQueue[0]->dir,
                    particleQueue[0]->spread,
                    particleQueue[0]->lifetime,
                    particleQueue[0]->window);
      }

      particleQueue.erase(particleQueue.begin());
      }
  }
}
