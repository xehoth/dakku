#ifndef DAKKU_SRC_LIGHTS_DIFFUSE_H_
#define DAKKU_SRC_LIGHTS_DIFFUSE_H_
#include <core/light.h>
#include <core/spectrum.h>
#include <core/primitive.h>

DAKKU_BEGIN
class DiffuseAreaLight : public AreaLight {
 public:
  DAKKU_DECLARE_OBJECT(DiffuseAreaLight);

  void construct(const Transform &lightToWorld, const Spectrum &_data,
                 int nSamples, const Shape *_shape, bool _twoSided);

  [[nodiscard]] Spectrum emit(const Interaction &it,
                              const Vector3f &w) const override;
  Spectrum sampleLi(const Interaction &ref, const Point2f &u, Vector3f &wi,
                    Float &pdf, VisibilityTester &vis) const override;
  [[nodiscard]] Float pdfLi(const Interaction &ref, const Interaction &lightIt,
                            bool foundIt, const Vector3f &wi) const override;
  void serialize(Json &json, OutputStream *stream) const override;
  void unserialize(const Json &json, InputStream *stream) override;
  [[nodiscard]] std::vector<Light *> getLightList() const override;
  [[nodiscard]] std::vector<Primitive *> getPrimitiveList() const override;
 private:
  Spectrum data;
  const Shape *shape{};
  bool twoSided{};
  Float area{};
  std::vector<std::unique_ptr<DiffuseAreaLight>> splitLights{};
  std::unique_ptr<GeometricPrimitive> primitive{};
};
DAKKU_END
#endif  // DAKKU_SRC_LIGHTS_DIFFUSE_H_
