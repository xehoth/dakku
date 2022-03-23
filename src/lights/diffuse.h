#ifndef DAKKU_SRC_LIGHTS_DIFFUSE_H_
#define DAKKU_SRC_LIGHTS_DIFFUSE_H_
#include <core/light.h>
#include <core/spectrum.h>

DAKKU_BEGIN
class DiffuseAreaLight : public AreaLight {
 public:
  DAKKU_DECLARE_OBJECT(DiffuseAreaLight);

  [[nodiscard]] Spectrum emit(const Interaction &it,
                              const Vector3f &w) const override;
  Spectrum sampleLi(const Interaction &ref, const Point2f &u, Vector3f &wi,
                    Float &pdf, VisibilityTester &vis) const override;
  void serialize(Json &json, OutputStream *stream) const override;
  void unserialize(const Json &json, InputStream *stream) override;

 private:
  Spectrum data;
  const Shape *shape{};
  bool twoSided{};
  Float area{};
};
DAKKU_END
#endif  // DAKKU_SRC_LIGHTS_DIFFUSE_H_
