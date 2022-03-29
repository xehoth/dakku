#ifndef DAKKU_SRC_CORE_SHAPE_H_
#define DAKKU_SRC_CORE_SHAPE_H_
#include <core/fwd.h>
#include <core/sobject.h>
#include <core/transform.h>

DAKKU_BEGIN

class Shape : public SerializableObject {
 public:
  DAKKU_DECLARE_OBJECT(Shape, SerializableObject);
  void construct(const Transform *objectToWorld,
                 const Transform *worldToObject);
  [[nodiscard]] virtual Float area() const = 0;

  // sample a point on the surface of the shape and return the pdf with
  // respect to area on the surface.
  virtual Interaction sample(const Point2f &u, Float &pdf) const = 0;
  [[nodiscard]] virtual Float pdf(const Interaction &) const {
    return 1 / area();
  }

  // sample a point on the shape given a reference point |ref| and
  // return the PDF with respect to solid angle from |ref|.
  virtual Interaction sample(const Interaction &ref, const Point2f &u,
                             Float &pdf) const;
  virtual Float pdf(const Interaction &ref, const Interaction &lightIt,
                    const Vector3f &wi) const;

  const Transform *objectToWorld{};
  const Transform *worldToObject{};
  bool transformSwapsHandedness{};
};

DAKKU_END
#endif  // DAKKU_SRC_CORE_SHAPE_H_
