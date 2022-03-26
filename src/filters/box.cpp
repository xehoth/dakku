#include <filters/box.h>

DAKKU_BEGIN

void BoxFilter::serialize(Json &json, OutputStream *) const {
  DAKKU_SER_J(radius);
  json["class"] = getClassName();
}

void BoxFilter::unserialize(const Json &json, InputStream *) {
  DAKKU_UNSER_JIE(radius, Vector2f(0.5, 0.5));
  Filter::construct(radius);
  DAKKU_INFO("create box filter with radius: {}", radius);
}

Float BoxFilter::evaluate(const Point2f &) const { return 1; }
DAKKU_END