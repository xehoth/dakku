#ifndef DAKKU_SRC_CORE_LIGHT_H_
#define DAKKU_SRC_CORE_LIGHT_H_
#include <core/sobject.h>
#include <core/transform.h>
#include <core/interaction.h>

DAKKU_BEGIN
/**
 * light flags
 */
enum class LightFlags : int {
  DELTA_POSITION = 1 << 0,
  DELTA_DIRECTION = 1 << 1,
  AREA = 1 << 2,
  INFINITE = 1 << 3
};

inline LightFlags operator|(const LightFlags &a, const LightFlags &b) {
  return static_cast<LightFlags>(static_cast<int>(a) | static_cast<int>(b));
}

inline bool isDeltaLight(const LightFlags &flags) {
  return static_cast<int>(flags) &
             static_cast<int>(LightFlags::DELTA_POSITION) ||
         static_cast<int>(flags) &
             static_cast<int>(LightFlags::DELTA_DIRECTION);
}

class Light : public SerializableObject {
 public:
  DAKKU_DECLARE_OBJECT(Light, SerializableObject);

  void construct(LightFlags flags, const Transform &lightToWorld,
                 int nSamples = 1);

  /**
   * sample a point on the light then return the incident radiance from the
   * light at a point ref.p and also return the direction vector wi that gives
   * the direction from which radiance is arriving
   * @param ref reference interaction
   * @param u random sample
   * @param[out] wi the radiance arriving direction
   * @param[out] pdf pdf of the sampled point
   * @param[out] vis visibility tester between ref.p and sampled light point
   * @return the radiance emitted
   */
  virtual Spectrum sampleLi(const Interaction &ref, const Point2f &u,
                            Vector3f &wi, Float &pdf,
                            VisibilityTester &vis) const = 0;
  [[nodiscard]] virtual Float pdfLi(const Interaction &ref,
                                    const Interaction &lightIt, bool foundIt,
                                    const Vector3f &wi) const = 0;
  virtual void preprocess(const Scene &scene) {}

  LightFlags flags{};
  int nSamples{1};

 protected:
  Transform lightToWorld, worldToLight;
};

class VisibilityTester {
 public:
  explicit VisibilityTester() = default;
  explicit VisibilityTester(const Interaction &p0, const Interaction &p1)
      : _p0(p0), _p1(p1) {}
  [[nodiscard]] const Interaction &p0() const { return _p0; }
  [[nodiscard]] const Interaction &p1() const { return _p1; }
  /**
   * determine whether p0 to p1 is unoccluded
   */
  [[nodiscard]] bool unoccluded(const Scene &scene) const;

 private:
  Interaction _p0, _p1;
};

class AreaLight : public Light {
 public:
  DAKKU_DECLARE_OBJECT(AreaLight, Light);

  void construct(const Transform &lightToWorld, int nSamples);
  /**
   * evaluate the area light's emitted radiance given the light sample point and
   * outgoing direction
   * @param it the light sample
   * @param w the outgoing direction
   * @return the emitted radiance
   */
  [[nodiscard]] virtual Spectrum emit(const Interaction &it,
                                      const Vector3f &w) const = 0;
  /**
   * some area lights (triangle mesh) needs to split into single triangles
   */
  [[nodiscard]] virtual std::vector<Light *> getLightList() const = 0;
  /**
   * corresponding primitive for lights
   */
  [[nodiscard]] virtual std::vector<Primitive *> getPrimitiveList() const = 0;
};

DAKKU_END
#endif  // DAKKU_SRC_CORE_LIGHT_H_
