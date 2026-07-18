#pragma once
#include <cstdint>

namespace Aleg {
  using CollisionGroup = uint32_t;

  namespace CollisionGroups {
    constexpr CollisionGroup Default = 1 << 0;
  }
}
