#ifndef DAKKU_SRC_LIGHTS_DISTANT_H_
#define DAKKU_SRC_LIGHTS_DISTANT_H_
#include <core/light.h>
#include <core/spectrum.h>

DAKKU_BEGIN
class DistantLight : public Light {
 public:
  DAKKU_DECLARE_OBJECT(DistantLight, Light);

  void construct(const Transform &lightToWorld, const Spectrum &_data,
                 const Vector3f &_wLight);
  Spectrum sampleLi(const Interaction &ref, const Point2f &u, Vector3f &wi,
                    Float &pdf, VisibilityTester &vis) const override;
  [[nodiscard]] Float pdfLi(const Interaction &ref, const Interaction &lightIt,
                            bool foundIt, const Vector3f &wi) const override {
    return 0;
  }
  void preprocess(const Scene &scene) override;
  void serialize(Json &json, OutputStream *stream) const override;
  void unserialize(const Json &json, InputStream *stream) override;

 private:
  Spectrum data;
  Vector3f wLight;  // light direction
  Point3f worldCenter;
  Float worldRadius{};
};
DAKKU_END
#endif  // DAKKU_SRC_LIGHTS_DISTANT_H_
