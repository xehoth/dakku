#ifndef DAKKU_SRC_FILTERS_TRIANGLE_H_
#define DAKKU_SRC_FILTERS_TRIANGLE_H_
#include <core/filter.h>

DAKKU_BEGIN
class TriangleFilter : public Filter {
 public:
  DAKKU_DECLARE_OBJECT(TriangleFilter, Filter);

  void serialize(Json &json, OutputStream *stream) const override;
  void unserialize(const Json &json, InputStream *stream) override;
  [[nodiscard]] Float evaluate(const Point2f &p) const override;
};
DAKKU_END
#endif  // DAKKU_SRC_FILTERS_TRIANGLE_H_
