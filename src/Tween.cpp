#include <Alegengine/helpers/Tween.hpp>

namespace Aleg {
  std::vector<TweenBase*> TweenBase::tweens;

  namespace Ease {
    float linear(float t) { return t; }
    float quad(float t) { return t * t; }
    float cubic(float t) { return t * t * t; }
    float quint(float t) { return t * t * t * t * t; }
    float sine(float t) { return 1.0f - std::cos((t * glm::pi<float>()) / 2.0f); }
    float back(float t) {
      float c1 = 1.70158f;
      float c3 = c1 + 1.0f;
      return c3 * t * t * t - c1 * t * t;
    }
    float expo(float t) { return t == 0.0f ? 0.0f : std::pow(2.0f, 10.0f * t - 10.0f); }
  }
}
