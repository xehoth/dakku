//
// Created by xehoth on 2021/10/2.
//

#ifndef DAKKU_INCLUDE_DAKKU_ACCELERATORS_EMBREE_ACCEL_H_
#define DAKKU_INCLUDE_DAKKU_ACCELERATORS_EMBREE_ACCEL_H_
#include <dakku/core/primitive.h>
#include <dakku/shapes/triangle.h>
#include <embree3/rtcore.h>
#include <memory>
#include <vector>

namespace dakku {

/**
 * Embree 3 Accelerator
 * Note: All primitive's shape should be triangle
 *
 */
class EmbreeAccel : public Aggregate {
 public:
  using TrianglePrimitivePair =
      std::pair<std::shared_ptr<TriangleMesh>,
                std::shared_ptr<std::vector<std::shared_ptr<Primitive>>>>;
  explicit EmbreeAccel(std::shared_ptr<std::vector<TrianglePrimitivePair>> p);
  ~EmbreeAccel() override;
  bool intersect(const Ray &r, SurfaceInteraction &isect) const override;
  bool occluded(const Ray &r) const override;

 private:
  RTCScene rtcScene{nullptr};
  RTCDevice rtcDevice{nullptr};
  std::shared_ptr<std::vector<TrianglePrimitivePair>> p{nullptr};

  [[nodiscard]] RTCRay toRtcRay(const Ray &ray) const;
};
}  // namespace dakku
#endif  // DAKKU_INCLUDE_DAKKU_ACCELERATORS_EMBREE_ACCEL_H_