#ifndef DAKKU_SRC_CORE_PRIMITIVE_H_
#define DAKKU_SRC_CORE_PRIMITIVE_H_
#include <core/sobject.h>
#include <core/bounds.h>
#include <span>

DAKKU_BEGIN
/**
 * dakku primitive
 */
class Primitive : public SerializableObject {
 public:
  DAKKU_DECLARE_OBJECT(Primitive);
  /**
   * get the intersection information between ray and primitive
   * @param r the given ray
   * @return whether there's intersection between r and this
   */
  virtual bool intersect(const Ray &r, SurfaceInteraction &) const = 0;
  /**
   * only get whether r and this intersects
   * @param r the given ray
   */
  [[nodiscard]] virtual bool intersectP(const Ray &r) const = 0;
  virtual void computeScatteringFunctions(SurfaceInteraction &si,
                                          MemoryArena &arena) const = 0;
  [[nodiscard]] virtual const AreaLight *getAreaLight() const = 0;
  [[nodiscard]] virtual const Material *getMaterial() const = 0;

  [[nodiscard]] virtual Bounds3f worldBound() const {
    DAKKU_ERR("unimplemented");
    return Bounds3f();
  }
};

class GeometricPrimitive : public Primitive {
 public:
  DAKKU_DECLARE_OBJECT(GeometricPrimitive);

  void construct(const Shape *shape, const Material *material,
                 const AreaLight *areaLight);

  bool intersect(const Ray &r, SurfaceInteraction &) const override;
  [[nodiscard]] bool intersectP(const Ray &r) const override;

  void computeScatteringFunctions(SurfaceInteraction &si,
                                  MemoryArena &arena) const override;
  [[nodiscard]] const AreaLight *getAreaLight() const override;
  [[nodiscard]] const Material *getMaterial() const override;
  [[nodiscard]] const Shape *getShape() const { return shape; }

  void serialize(Json &json, OutputStream *stream) const override;
  void unserialize(const Json &json, InputStream *stream) override;

  friend class RenderState;

 protected:
  const Shape *shape{};
  const Material *material{};
  const AreaLight *areaLight{};
};

class Aggregate : public Primitive {
 public:
  DAKKU_DECLARE_OBJECT(Aggregate);
  /**
   * some aggregate (accelerators) needs
   */
  virtual void build(std::span<const Primitive *> primitives) = 0;
  [[nodiscard]] const AreaLight *getAreaLight() const override;
  [[nodiscard]] const Material *getMaterial() const override;
  void computeScatteringFunctions(SurfaceInteraction &si,
                                  MemoryArena &arena) const override;
};
DAKKU_END
#endif  // DAKKU_SRC_CORE_PRIMITIVE_H_
