#ifndef DAKKU_CORE_SHAPE_H_
#define DAKKU_CORE_SHAPE_H_
#include <core/transform.h>
#include <core/object.h>
#include <core/bounds.h>

namespace dakku {

/**
 * @brief shape
 * 
 */
class DAKKU_EXPORT_CORE Shape : public Object {
 public:
  DAKKU_DECLARE_OBJECT(Shape, Object);

  /**
   * @brief Construct a new Shape object with given transform
   *
   */
  explicit Shape(const Transform *objectToWorld,
                 const Transform *worldToObject);

  /**
   * @brief given objectToWorld, calculate inverse for worldToObject
   *
   */
  explicit Shape(const Transform *objectToWorld);

  /**
   * @brief get object bounding box
   *
   */
  [[nodiscard]] virtual Bounds3f objectBound() const = 0;

  /**
   * @brief get world bounding box
   *
   */
  [[nodiscard]] virtual Bounds3f worldBound() const;

  /**
   * @brief shape-ray intersection
   *
   * @param ray given ray
   * @param [out] tHit hit ray time
   * @param [out] isect intersect info
   */
  virtual bool intersect(const Ray &ray, float *tHit, SurfaceInteraction *isect,
                         bool testAlphaTexture = true) const = 0;

  /**
   * @brief check whether ray and shape has intersection
   *
   */
  [[nodiscard]] virtual bool hasIntersection(
      const Ray &ray, bool testAlphaTexture = true) const;

  /**
   * @brief get shape area
   *
   */
  [[nodiscard]] virtual float area() const = 0;

  /**
   * @brief sample a point on the shape and return the pdf with respect to the
   * area of the surface
   *
   * @param u random sample
   * @param pdf pdf
   * @return the sampled point
   */
  virtual Interaction samplePosition(const Point2f &u, float &pdf) const = 0;

  /**
   * @brief the pdf of sampling a point on the shape with respect to the area of
   * the surface
   *
   */
  [[nodiscard]] virtual float pdfPosition(const Interaction &) const;

  /**
   * @brief sample a point on the shape and return the pdf with respect to solid
   * angle from given `ref`
   *
   * $p(\omega) = \left(\frac{\mathrm d \omega}{\mathrm d x}\right) ^ {-1} p(x)$
   *
   * $\mathrm d \omega = \mathrm d x \frac{|N(x) \cdot -\vec r|}{||\vec r|| ^ 2}$
   * where $\vec r = x - \text{ref}.x$
   *
   * $\Longrightarrow p(\omega) = \frac{||\vec r|| ^ 2}{|N(x) \cdot -\vec r|}p(x)$
   *
   * @param ref reference point
   * @param u random sample
   * @param pdf pdf
   */
  virtual Interaction sampleDirection(const Interaction &ref, const Point2f &u,
                                      float &pdf) const;

  /**
   * @brief the pdf of sampling a point on the shape with respect to the area of
   * the surface
   *
   * @param ref the ref point
   * @param wi direction
   */
  [[nodiscard]] virtual float pdfDirection(const Interaction &ref,
                                           const Vector3f &wi) const;

  const Transform *objectToWorld, *worldToObject;
  const bool transformSwapsHandedness;
};
}  // namespace dakku
#endif