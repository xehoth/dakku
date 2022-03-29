#ifndef DAKKU_SRC_ACCELERATORS_EMBREE_ACCEL_H_
#define DAKKU_SRC_ACCELERATORS_EMBREE_ACCEL_H_
#include <core/primitive.h>
#include <shapes/triangle.h>
#include <embree3/rtcore.h>
#include <vector>
#include <span>

DAKKU_BEGIN
/**
 * Embree 3 Accelerator
 * Note: All primitive's shape should be triangle (currently)
 *
 */
class EmbreeAccel : public Aggregate {
 public:
  DAKKU_DECLARE_OBJECT(EmbreeAccel);

  void serialize(Json &json, OutputStream *) const override;
  void unserialize(const Json &json, InputStream *) override;

  explicit EmbreeAccel(std::span<const Primitive *> primitives);
  void build(std::span<const Primitive *> primitives) override;
  ~EmbreeAccel() override;
  bool intersect(const Ray &r, SurfaceInteraction &) const override;
  [[nodiscard]] bool intersectP(const Ray &r) const override;
  [[nodiscard]] Bounds3f worldBound() const override;

 private:
  RTCScene rtcScene{};
  RTCDevice rtcDevice{};
  std::vector<const TriangleMeshPrimitive *> primitives;
  std::vector<const TriangleMesh *> triangles;
};
DAKKU_END
#endif  // DAKKU_SRC_ACCELERATORS_EMBREE_ACCEL_H_
