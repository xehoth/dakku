#include <filters/triangle.h>

DAKKU_BEGIN

void TriangleFilter::serialize(Json &json, OutputStream *) const {
  DAKKU_SER_J(radius);
  json["class"] = getClassName();
}

void TriangleFilter::unserialize(const Json &json, InputStream *stream) {
  DAKKU_UNSER_JIE(radius, Vector2f(2.0, 2.0));
  Filter::construct(radius);
  DAKKU_INFO("create triangle filter with radius: {}", radius);
}

Float TriangleFilter::evaluate(const Point2f &p) const {
  return std::max(static_cast<Float>(0), radius.x() - std::abs(p.x())) *
         std::max(static_cast<Float>(0), radius.y() - std::abs(p.y()));
}
DAKKU_END
