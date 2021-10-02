//
// Created by xehoth on 2021/10/2.
//

#ifndef DAKKU_INCLUDE_DAKKU_CORE_LIGHT_H_
#define DAKKU_INCLUDE_DAKKU_CORE_LIGHT_H_
#include <dakku/core/dakku.h>
#include <dakku/core/geometry.h>

namespace dakku {

enum class LightFlags : int {
  DELTA_POSITION = 1 << 0,
  DELTA_DIRECTION = 1 << 1,
  AREA = 1 << 2,
  INFINITE = 1 << 3,
};

inline int operator&(const LightFlags &a, const LightFlags &b) {
  return static_cast<int>(a) & static_cast<int>(b);
}

inline bool isDeltaLight(LightFlags flags) {
  return flags & LightFlags::DELTA_POSITION ||
         flags & LightFlags::DELTA_DIRECTION;
}

class Light {
 public:
  virtual ~Light() = default;
  explicit Light(LightFlags flags);

  const LightFlags flags;
};

// class VisibilityTester {
//  public:
//   VisibilityTester() = default;
//   VisibilityTester(const Interaction &p0, const Interaction &p1)
//       : p0(p0), p1(p1) {}
//
//  private:
//   Interaction p0, p1;
// };

class AreaLight : public Light {
 public:
  AreaLight();
  virtual RGBSpectrum emit(const Interaction &intr,
                           const Vector3f &w) const = 0;
};
}  // namespace dakku
#endif  // DAKKU_INCLUDE_DAKKU_CORE_LIGHT_H_