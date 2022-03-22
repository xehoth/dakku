#ifndef DAKKU_SRC_CORE_PRIMITIVE_H_
#define DAKKU_SRC_CORE_PRIMITIVE_H_
#include <core/sobject.h>

DAKKU_BEGIN
class Primitive : public SerializableObject {
 public:
  virtual bool intersect(const Ray &r, SurfaceInteraction &) const = 0;
  [[nodiscard]] virtual bool intersectP(const Ray &r) const = 0;
};

class GeometricPrimitive : public Primitive {
 public:
  DAKKU_DECLARE_OBJECT(GeometricPrimitive);

  bool intersect(const Ray &r, SurfaceInteraction &) const override;
  [[nodiscard]] bool intersectP(const Ray &r) const override;
  void serialize(Json &json, OutputStream *stream) const override;
  void unserialize(const Json &json, InputStream *stream) override;

 private:
  Shape *shape{};
  AreaLight *areaLight{};
};
DAKKU_END
#endif  // DAKKU_SRC_CORE_PRIMITIVE_H_
