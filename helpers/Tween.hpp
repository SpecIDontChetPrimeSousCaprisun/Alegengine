#pragma once
#include <functional>
#include <cmath>
#include <vector>

namespace Aleg {
  enum class EaseDirection { In, Out, InOut };

  class TweenBase {
  public:
    virtual void update() {}
    virtual ~TweenBase() = default;

    static std::vector<TweenBase*> tweens;
    static void updateAll() {
      for (TweenBase* tween : tweens) tween->update();
    }
  };

  template <typename T>
  class Tween : public TweenBase {
  public:
    Tween(T* target, T from, T to, float duration, Window* window,
          std::function<float(float)> easing, EaseDirection dir)
      : target(target), from(from), to(to), duration(duration),
        window(window), easing(easing), dir(dir) {
      if (!this->window) this->window = Window::currentWindow;
      tweens.push_back(this);
    }

    void play() {
      isPlaying = true;
      time = 0.0f;
    }

    void update() override {
      if (!isPlaying) return;
      time += window->deltaTime;

      float t = std::min(time / duration, 1.0f);
      float eased;

      if (dir == EaseDirection::Out) eased = easeOut(easing, t);
      else if (dir == EaseDirection::InOut) eased = easeInOut(easing, t);
      else eased = easing(t);

      *target = from + (to - from) * eased;
      if (time >= duration) {
        *target = to;
        isPlaying = false;
      }
    }

  private:
    T* target;
    T from, to;
    float duration;
    float time = 0.0f;
    bool isPlaying = false;
    Window* window;
    std::function<float(float)> easing;
    EaseDirection dir;

    static float easeOut(std::function<float(float)> easeInFn, float t) {
      return 1.0f - easeInFn(1.0f - t);
    }

    static float easeInOut(std::function<float(float)> easeInFn, float t) {
      if (t < 0.5f) return easeInFn(t * 2.0f) * 0.5f;
      return 1.0f - easeInFn((1.0f - t) * 2.0f) * 0.5f;
    }
  };
 
  namespace Ease {
    float linear(float t);
    float quint(float t);
    float quad(float t);
    float sine(float t);
    float cubic(float t);
    float back(float t);
    float expo(float t);
  }
}
