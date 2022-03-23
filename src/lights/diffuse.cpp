#include <lights/diffuse.h>
#include <core/vector.h>
#include <core/interaction.h>
#include <core/shape.h>
#include <core/state.h>

DAKKU_BEGIN

Spectrum DiffuseAreaLight::emit(const Interaction &it,
                                const Vector3f &w) const {
  return (twoSided || it.n.dot(w) > 0) ? data : Spectrum{0};
}

void DiffuseAreaLight::serialize(Json &json, OutputStream *) const {
  json["class"] = getClassNameStatic();
  DAKKU_SER_J(data);
  DAKKU_SER_J(twoSided);
  DAKKU_SER_J(nSamples);
  std::string shapeName;
  // TODO: optimize this
  for (const auto &[k, v] : renderState.shapes) {
    if (v.get() == this->shape) {
      shapeName = k;
      break;
    }
  }
  json["shape"] = shapeName;
  // TODO: transform
}

void DiffuseAreaLight::unserialize(const Json &json, InputStream *) {
  DAKKU_UNSER_J(data);
  DAKKU_UNSER_JI(twoSided);
  DAKKU_UNSER_JI(nSamples);
  std::string shapeName;
  if (!json.contains("shape")) {
    DAKKU_ERR("diffuse area light requires a shape!");
  } else {
    json.at("shape").get_to(shapeName);
    if (auto it = renderState.shapes.find(shapeName);
        it != renderState.shapes.end()) {
      this->shape = it->second.get();
    } else {
      DAKKU_ERR("cannot find shape {} in shape map", shapeName);
    }
  }
  // TODO: transform
  AreaLight::construct(Transform(), nSamples);
}

Spectrum DiffuseAreaLight::sampleLi(const Interaction &ref, const Point2f &u,
                                    Vector3f &wi, Float &pdf,
                                    VisibilityTester &vis) const {
  Interaction pShape = shape->sample(ref, u, pdf);
  if (pdf == 0 || (pShape.p - ref.p).squaredNorm() == 0) {
    pdf = 0;
    return Spectrum{0};
  }
  wi = (pShape.p - ref.p).normalized();
  vis = VisibilityTester(ref, pShape);
  return emit(pShape, -wi);
}

DAKKU_END
