#include <lights/diffuse.h>
#include <core/vector.h>
#include <core/interaction.h>
#include <core/shape.h>
#include <core/state.h>
#include <shapes/triangle.h>

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
  const Material *pMaterial{};
  std::string materialName;
  if (!json.contains("material")) {
    DAKKU_ERR("diffuse area light requires a material!");
  } else {
    json.at("material").get_to(materialName);
    if (auto it = renderState.materials.find(materialName);
        it != renderState.materials.end()) {
      pMaterial = it->second.get();
    } else {
      DAKKU_ERR("cannot find material {} in material map", materialName);
    }
  }
  if (this->shape->getClassName() == "TriangleMesh") {
    auto triPrim = std::make_unique<TriangleMeshPrimitive>();
    triPrim->construct(this->shape, pMaterial, this);
    const auto *triMesh = dynamic_cast<const TriangleMesh *>(this->shape);
    splitLights.reserve(triMesh->getNumTriangles());
    for (int i = 0; i < triMesh->getNumTriangles(); ++i) {
      auto ptr = std::make_unique<DiffuseAreaLight>();
      ptr->construct(this->lightToWorld, this->data, this->nSamples,
                     triMesh->getTriangle(i), this->twoSided);
      splitLights.emplace_back(std::move(ptr));
    }
    triPrim->primTriangles =
        std::make_unique<GeometricPrimitive[]>(triMesh->getNumTriangles());
    for (int i = 0; i < triMesh->getNumTriangles(); ++i) {
      triPrim->primTriangles[i].construct(triMesh->getTriangle(i), pMaterial,
                                          this->splitLights[i].get());
    }
    this->primitive = std::move(triPrim);
  } else if (this->shape->isDerivedFrom("TriangleMesh")) {
    this->construct(this->lightToWorld, this->data, this->nSamples, this->shape,
                    this->twoSided);
    // use triangles to intersect but overrides sample
    auto triPrim = std::make_unique<TriangleMeshPrimitive>();
    triPrim->construct(this->shape, pMaterial, this);
    const auto *triMesh = dynamic_cast<const TriangleMesh *>(this->shape);
    triPrim->primTriangles =
        std::make_unique<GeometricPrimitive[]>(triMesh->getNumTriangles());
    for (int i = 0; i < triMesh->getNumTriangles(); ++i) {
      triPrim->primTriangles[i].construct(triMesh->getTriangle(i), pMaterial,
                                          this);
    }
    this->primitive = std::move(triPrim);
  } else {
    DAKKU_ERR("unimplemented other shapes");
  }
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

Float DiffuseAreaLight::pdfLi(const Interaction &ref,
                              const Interaction &lightIt, bool foundIt,
                              const Vector3f &wi) const {
  if (!foundIt) return 0;
  return shape->pdf(ref, lightIt, wi);
}

void DiffuseAreaLight::construct(const Transform &lightToWorld,
                                 const Spectrum &_data, int nSamples,
                                 const Shape *_shape, bool _twoSided) {
  AreaLight::construct(lightToWorld, nSamples);
  this->data = _data;
  this->nSamples = nSamples;
  this->shape = _shape;
  this->twoSided = _twoSided;
  this->area = shape->area();
}

std::vector<Light *> DiffuseAreaLight::getLightList() const {
  std::vector<Light *> ret;
  if (!splitLights.empty()) {
    ret.resize(splitLights.size());
    for (size_t i = 0; i < splitLights.size(); ++i)
      ret[i] = splitLights[i].get();
  } else {
    ret.push_back(const_cast<DiffuseAreaLight *>(this));
  }
  return ret;
}

std::vector<Primitive *> DiffuseAreaLight::getPrimitiveList() const {
  std::vector<Primitive *> ret;
  if (primitive->isDerivedFrom("TriangleMeshPrimitive")) {
    //    auto *prim = dynamic_cast<TriangleMeshPrimitive *>(primitive.get());
    //    const auto *tri = dynamic_cast<const TriangleMesh *>(this->shape);
    //    ret.resize(tri->getNumTriangles());
    //    for (size_t i = 0; i < ret.size(); ++i) ret[i] =
    //    &prim->primTriangles[i];
    ret.push_back(primitive.get());
  } else {
    DAKKU_ERR("unimplemented");
  }
  return ret;
}

DAKKU_END
