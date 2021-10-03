//
// Created by xehoth on 2021/10/2.
//

#ifndef DAKKU_INCLUDE_DAKKU_CORE_LIGHT_H_
#define DAKKU_INCLUDE_DAKKU_CORE_LIGHT_H_
#include <dakku/core/dakku.h>
#include <dakku/core/geometry.h>
#include <dakku/core/interaction.h>
#include <dakku/core/transform.h>
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
  explicit Light(LightFlags flags, const Transform &lightToWorld);
  virtual RGBSpectrum sampleLi(const Interaction &ref, const Point2f &u,
                               Vector3f &wi, Float &pdf,
                               VisibilityTester &vis) const = 0;

  const LightFlags flags;

 protected:
  const Transform lightToWorld, worldToLight;
};

class VisibilityTester {
 public:
  VisibilityTester() = default;
  VisibilityTester(const Interaction &p0, const Interaction &p1)
      : p0(p0), p1(p1) {}
  [[nodiscard]] bool unoccluded(const Scene &scene) const;

 private:
  Interaction p0, p1;
};

class AreaLight : public Light {
 public:
  explicit AreaLight(const Transform &lightToWorld);
  virtual RGBSpectrum emit(const Interaction &intr,
                           const Vector3f &w) const = 0;
};
}  // namespace dakku
#endif  // DAKKU_INCLUDE_DAKKU_CORE_LIGHT_H_
