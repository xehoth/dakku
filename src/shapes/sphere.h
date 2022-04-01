#ifndef DAKKU_SRC_SHAPES_SPHERE_H_
#define DAKKU_SRC_SHAPES_SPHERE_H_
#include <shapes/triangle.h>

DAKKU_BEGIN
class Sphere : public TriangleMesh {
 public:
  DAKKU_DECLARE_OBJECT(Sphere, TriangleMesh);
  Interaction sample(const Point2f &u, Float &pdf) const override;
  [[nodiscard]] Float area() const override;
  void serialize(Json &json, OutputStream *stream) const override;
  void unserialize(const Json &json, InputStream *stream) override;

 private:
  void generateMesh();
  Point3f o;
  Float radius{1};
  int subdivisions{5};
};
DAKKU_END
#endif  // DAKKU_SRC_SHAPES_SPHERE_H_
