#include <core/primitive.h>
#include <core/logger.h>
#include <core/state.h>

DAKKU_BEGIN

bool GeometricPrimitive::intersect(const Ray &, SurfaceInteraction &) const {
  DAKKU_ERR("intersect has not been implemented yet! Use aggregate instead");
  return false;
}
bool GeometricPrimitive::intersectP(const Ray &) const {
  DAKKU_ERR("intersect has not been implemented yet! Use aggregate instead");
  return false;
}

void GeometricPrimitive::serialize(Json &json, OutputStream *stream) const {
  // TODO: optimize this
  std::string shapeName;
  for (const auto &[name, p] : renderState.shapes) {
    if (p.get() == shape) {
      shapeName = name;
    }
  }
  json["shape"] = shapeName;

}

void GeometricPrimitive::unserialize(const Json &json, InputStream *stream) {}

DAKKU_END
