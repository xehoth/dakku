//
// Created by xehoth on 2021/10/2.
//

#ifndef DAKKU_INCLUDE_DAKKU_CORE_PRIMITIVE_H_
#define DAKKU_INCLUDE_DAKKU_CORE_PRIMITIVE_H_
#include <dakku/core/dakku.h>
#include <dakku/core/shape.h>
#include <memory>

namespace dakku {

class Primitive {
 public:
  virtual ~Primitive() = default;
  virtual bool intersect(const Ray &r, SurfaceInteraction &isect) const = 0;
  [[nodiscard]] virtual bool occluded(const Ray &r) const = 0;
  virtual void computeScatteringFunctions(SurfaceInteraction &isect) const = 0;
  [[nodiscard]] virtual std::shared_ptr<const Material> getMaterial() const = 0;
  [[nodiscard]] virtual std::shared_ptr<const AreaLight> getAreaLight()
      const = 0;
};

class GeometricPrimitive : public Primitive {
 public:
  explicit GeometricPrimitive(const std::shared_ptr<Shape> &shape,
                              std::shared_ptr<Material> material,
                              std::shared_ptr<AreaLight> areaLight);
  bool intersect(const Ray &r, SurfaceInteraction &isect) const override;
  [[nodiscard]] bool occluded(const Ray &r) const override;
  void computeScatteringFunctions(SurfaceInteraction &isect) const override;
  [[nodiscard]] std::shared_ptr<const Material> getMaterial() const override;
  [[nodiscard]] std::shared_ptr<const AreaLight> getAreaLight() const override;

 private:
  std::shared_ptr<Shape> shape{nullptr};
  std::shared_ptr<Material> material{nullptr};
  std::shared_ptr<AreaLight> areaLight{nullptr};
};

class Aggregate : public Primitive {
 public:
  void computeScatteringFunctions(SurfaceInteraction &isect) const override;
  [[nodiscard]] std::shared_ptr<const AreaLight> getAreaLight() const override;
  [[nodiscard]] std::shared_ptr<const Material> getMaterial() const override;
};
}  // namespace dakku
#endif  // DAKKU_INCLUDE_DAKKU_CORE_PRIMITIVE_H_
