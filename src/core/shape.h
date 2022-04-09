#ifndef DAKKU_CORE_SHAPE_H_
#define DAKKU_CORE_SHAPE_H_
#include <core/transform.h>
#include <core/object.h>
#include <core/bounds.h>

namespace dakku {

class DAKKU_EXPORT_CORE Shape : public Object {
 public:
  DAKKU_DECLARE_OBJECT(Shape, Object);

  /**
   * @brief Construct a new Shape object with given transform
   * 
   */
  explicit Shape(const Transform *objectToWorld, const Transform *worldToObject);

  /**
   * @brief get object bounding box
   * 
   */
  [[nodiscard]] virtual Bounds3f objectBound() const = 0;

  /**
   * @brief get world bounding box
   * 
   */
  [[nodiscard]] virtual Bounds3f worldBound() const = 0;
};
}  // namespace dakku
#endif