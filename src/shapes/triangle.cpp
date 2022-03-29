#include <shapes/triangle.h>
#include <core/logger.h>
#include <core/sampling.h>
#include <core/interaction.h>
#include <core/stream.h>
#include <core/transform_cache.h>
#include <core/relative.h>
#include <core/state.h>
#include <core/light.h>
#include <igl/readOBJ.h>
#include <igl/readPLY.h>
#include <igl/per_vertex_normals.h>
#include <filesystem>

DAKKU_BEGIN
void Triangle::serialize(Json &, OutputStream *) const {
  DAKKU_ERR("cannot serialize triangle, use triangle mesh instead");
}

void Triangle::unserialize(const Json &, InputStream *) {
  DAKKU_ERR("cannot unserialize triangle, use triangle mesh instead");
}

Triangle::Triangle(const Transform *objectToWorld,
                   const Transform *worldToObject, const TriangleMesh *mesh,
                   int triNumber) {
  Shape::construct(objectToWorld, worldToObject);
  this->mesh = mesh;
  this->v = &mesh->indices[3 * triNumber];
}

Float Triangle::area() const {
  const Point3f &p0 = mesh->p[v[0]];
  const Point3f &p1 = mesh->p[v[1]];
  const Point3f &p2 = mesh->p[v[2]];
  return static_cast<Float>(0.5) * (p1 - p0).cross(p2 - p0).norm();
}

Interaction Triangle::sample(const Point2f &u, Float &pdf) const {
  Point2f b = uniformSampleTriangle(u);

  const Point3f &p0 = mesh->p[v[0]];
  const Point3f &p1 = mesh->p[v[1]];
  const Point3f &p2 = mesh->p[v[2]];
  Interaction it;

  it.p = Point3f((1 - b[0] - b[1]) * p0 + b[0] * p1 + b[1] * p2);
  // compute surface normal for sampled point on triangle
  it.n = Normal3f((p1 - p0).cross(p2 - p0)).normalized();
  // ensure correct orientation of the geometric normal
  if (mesh->n) {
    Normal3f ns = Normal3f((1 - b[0] - b[1]) * Vector3f(mesh->n[v[0]]) +
                           b[0] * Vector3f(mesh->n[v[1]]) +
                           b[1] * Vector3f(mesh->n[v[2]]))
                      .normalized();
    it.n = faceforward(it.n, ns);
  } else if (transformSwapsHandedness) {
    it.n = -it.n;
  }
  pdf = 1 / area();
  return it;
}

void Triangle::computeInteraction(Float b0, Float b1, Float b2,
                                  SurfaceInteraction &isect) const {
  const Point3f &p0 = mesh->p[v[0]];
  const Point3f &p1 = mesh->p[v[1]];
  const Point3f &p2 = mesh->p[v[2]];
  Point2f uv[3];
  getTexCoords(uv);
  Vector3f dpdu, dpdv;
  {
    // point in triangle can be described as p_o + u dpdu + v dpdv for some p_o
    // p_i = p_o + u_i dpdu + v_i dpdv
    // => p0 - p2 = (u0 - u2) dpdu + (v0 - v2)dpdv
    // => p1 - p2 = (u1 - u2) dpdu + (v1 - v2)dpdv
    // compute deltas for triangle partial derivatives
    Vector2f duv02 = uv[0] - uv[2], duv12 = uv[1] - uv[2];
    Vector3f dp02 = p0 - p2, dp12 = p1 - p2;
    Float determinant = duv02[0] * duv12[1] - duv02[1] * duv12[0];
    bool degenerateUV = std::abs(determinant) < 1e-8;
    if (!degenerateUV) {
      Float invDet = 1 / determinant;
      dpdu = (duv12[1] * dp02 - duv02[1] * dp12) * invDet;
      dpdv = (-duv12[0] * dp02 + duv02[0] * dp12) * invDet;
    }
    if (degenerateUV || dpdu.cross(dpdv).squaredNorm() == 0) {
      // handle zero determinant for triangle partial derivative matrix
      Vector3f ng = (p2 - p0).cross(p1 - p0);
      coordinateSystem(ng.normalized(), dpdu, dpdv);
    }
  }
  // interpolate (u, v) parametric coordinates and hit point
  Point3f pHit = b0 * p0 + b1 * p1 + b2 * p2;
  Point2f uvHit = b0 * uv[0] + b1 * uv[1] + b2 * uv[2];
  isect.p = pHit;
  isect.uv = uvHit;
  isect.dpdu = dpdu;
  isect.dpdv = dpdv;
  if (isect.shape && isect.shape->transformSwapsHandedness) isect.n = -isect.n;
  isect.shading.n = isect.n;
  Normal3f ns = getShadingNormal(b1, b2);
  // shading tangent
  Vector3f ss = dpdu.normalized();
  // shading bitangent
  Vector3f ts = ss.cross(Vector3f(ns));
  if (ts.squaredNorm() > 0) {
    ts.normalize();
    ss = ts.cross(Vector3f(ns));
  } else {
    coordinateSystem(Vector3f(ns), ss, ts);
  }
  // compute dndu, dndv
  Normal3f dndu, dndv;
  {
    // similar to calculating dpdu and dpdv
    // compute deltas for triangle partial derivatives of normal
    Vector2f duv02 = uv[0] - uv[2];
    Vector2f duv12 = uv[1] - uv[2];
    Normal3f dn1 = mesh->n[v[0]] - mesh->n[v[2]];
    Normal3f dn2 = mesh->n[v[1]] - mesh->n[v[2]];
    Float determinant = duv02[0] * duv12[1] - duv02[1] * duv12[0];
    bool degenerateUV = std::abs(determinant) < 1e-8;
    if (degenerateUV) {
      // we can still compute dndu and dndv, with respect to the
      // same arbitrary coordinate system we use to compute dpdu
      // and dpdv when this happens. It's important to do this
      // (rather than giving up) so that ray differentials for
      // rays reflected from triangles with degenerate
      // parameterizations are still reasonable.
      Vector3f dn = Vector3f(mesh->n[v[2]] - mesh->n[v[0]])
                        .cross(Vector3f(mesh->n[v[1]] - mesh->n[v[0]]));
      if (dn.squaredNorm() == 0) {
        dndu = dndv = Normal3f(0, 0, 0);
      } else {
        Vector3f dnu, dnv;
        coordinateSystem(dn, dnu, dnv);
        dndu = Normal3f(dnu);
        dndv = Normal3f(dnv);
      }
    } else {
      Float invDet = 1 / determinant;
      dndu = (duv12[1] * dn1 - duv02[1] * dn2) * invDet;
      dndv = (-duv12[0] * dn1 + duv02[0] * dn2) * invDet;
    }
  }
  isect.setShadingGeometry(ss, ts, dndu, dndv, true);
}

void TriangleMesh::serialize(Json &json, OutputStream *stream) const {
  json["data"] = "serialized";
  json["class"] = getClassNameStatic();
  stream->writeBytes(&nTriangles, sizeof(nTriangles));
  stream->writeBytes(&nVertices, sizeof(nVertices));
  stream->writeBytes(indices.get(), sizeof(int) * nTriangles * 3);
  stream->writeBytes(p.get(), sizeof(Point3f) * nVertices);
  stream->writeBytes(n.get(), sizeof(Point2f) * nVertices);
  std::uint8_t hasUv = uv != nullptr;
  stream->writeByte(hasUv);
  if (hasUv) stream->writeBytes(uv.get(), sizeof(Point2f) * nVertices);
  stream->writeBytes(objectToWorld, sizeof(Transform));
  stream->writeBytes(worldToObject, sizeof(Transform));
}

void TriangleMesh::unserialize(const Json &json, InputStream *stream) {
  // data
  if (stream && json["data"] == "serialized") {
    DAKKU_DEBUG("unserialize triangle mesh from serialized data");
    stream->readBytes(&nTriangles, sizeof(nTriangles));
    stream->readBytes(&nVertices, sizeof(nVertices));
    indices = std::make_unique<int[]>(nTriangles * 3);
    stream->readBytes(indices.get(), sizeof(int) * nTriangles * 3);
    p = std::make_unique<Point3f[]>(nVertices);
    stream->readBytes(p.get(), sizeof(Point3f) * nVertices);
    n = std::make_unique<Normal3f[]>(nVertices);
    stream->readBytes(n.get(), sizeof(Normal3f) * nVertices);
    std::uint8_t hasUv = stream->readByte();
    if (hasUv) {
      uv = std::make_unique<Point2f[]>(nVertices);
      stream->readBytes(uv.get(), sizeof(Point2f) * nVertices);
    }
  } else {
    loadMesh(json["data"]);
  }
  // transform
  Transform toWorld, toObject;
  if (stream && json.contains("transform") &&
      json["transform"] == "serialized") {
    DAKKU_DEBUG("unserialize transform from serialized data");
    stream->readBytes(&toWorld, sizeof(Transform));
    stream->readBytes(&toObject, sizeof(Transform));
  } else {
    DAKKU_WARN("parsing transform has not been implemented yet!");
  }
  const Transform *pToWorld = TransformCache::instance().lookUp(toWorld);
  const Transform *pToObject = TransformCache::instance().lookUp(toObject);
  this->construct(pToWorld, pToObject);

  // create triangles
  triangles = std::make_unique<Triangle[]>(nTriangles);
  for (int i = 0; i < nTriangles; ++i)
    triangles[i] = Triangle(pToWorld, pToObject, this, i);
  DAKKU_INFO("load triangle mesh, vertices: {}, faces: {}", nVertices,
             nTriangles);
}

void TriangleMesh::loadMesh(const std::string &fileName) {
  std::filesystem::path path = RelativeRoot::instance().get() + fileName;
  if (!std::filesystem::exists(path)) {
    DAKKU_ERR("mesh file does not exists: {}", fileName);
    return;
  }
  auto ext = path.extension();
  if (ext == ".obj") {
    Eigen::MatrixXd V, TC, N;
    Eigen::MatrixXi F, FTC, FN;
    if (!igl::readOBJ(path.string(), V, TC, N, F, FTC, FN)) {
      DAKKU_ERR("failed to read obj: {}", path);
    }
    if (N.rows() == 0) {
      DAKKU_INFO(
          "normal is not provided in the mesh, use calculated per vertex "
          "normal instead!");
      igl::per_vertex_normals(V, F, N);
    }

    Eigen::Matrix<Float, Eigen::Dynamic, Eigen::Dynamic> f_V =
        V.cast<Float>().transpose();
    Eigen::Matrix<Float, Eigen::Dynamic, Eigen::Dynamic> f_N =
        N.cast<Float>().transpose();
    Eigen::Matrix<Float, Eigen::Dynamic, Eigen::Dynamic> f_TC =
        TC.cast<Float>().transpose();
    F.transposeInPlace();
    assign(
        std::span<const int>{F.data(), static_cast<size_t>(F.size())},
        std::span<const Float>{f_V.data(), static_cast<size_t>(f_V.size())},
        std::span<const Float>{f_N.data(), static_cast<size_t>(f_N.size())},
        std::span<const Float>{f_TC.data(), static_cast<size_t>(f_TC.size())});
  } else if (ext == ".ply") {
    Eigen::MatrixXd V, N, TC;
    Eigen::MatrixXi F;
    if (!igl::readPLY(path.string(), V, F, N, TC)) {
      DAKKU_ERR("failed to read ply: {}", path);
    }
    if (N.rows() == 0) {
      DAKKU_INFO(
          "normal is not provided in the mesh, use calculated per vertex "
          "normal instead!");
      igl::per_vertex_normals(V, F, N);
    }
    Eigen::Matrix<Float, Eigen::Dynamic, Eigen::Dynamic> f_V =
        V.cast<Float>().transpose();
    Eigen::Matrix<Float, Eigen::Dynamic, Eigen::Dynamic> f_N =
        N.cast<Float>().transpose();
    Eigen::Matrix<Float, Eigen::Dynamic, Eigen::Dynamic> f_TC =
        TC.cast<Float>().transpose();
    F.transposeInPlace();
    assign(
        std::span<const int>{F.data(), static_cast<size_t>(F.size())},
        std::span<const Float>{f_V.data(), static_cast<size_t>(f_V.size())},
        std::span<const Float>{f_N.data(), static_cast<size_t>(f_N.size())},
        std::span<const Float>{f_TC.data(), static_cast<size_t>(f_TC.size())});
  } else {
    DAKKU_ERR("unsupported mesh file: {}", ext);
  }
}

void TriangleMesh::assign(std::span<const int> _i, std::span<const Float> _p,
                          std::span<const Float> _n,
                          std::span<const Float> _uv) {
  DAKKU_CHECK(_i.size() % 3 == 0,
              "invalid triangle mesh: num of indices is not multiples of 3");
  DAKKU_CHECK(
      _p.size() % 3 == 0,
      "invalid triangle mesh: vertices stream size is not multiples of 3");
  nTriangles = static_cast<int>(_i.size() / 3);
  nVertices = static_cast<int>(_p.size() / 3);
  DAKKU_CHECK(_n.size() == nVertices * 3, "wrong normal size");
  indices = std::make_unique<int[]>(3 * nTriangles);
  p = std::make_unique<Point3f[]>(nVertices);
  n = std::make_unique<Normal3f[]>(nVertices);
  std::copy(_i.begin(), _i.end(), indices.get());
  for (int i = 0; i < nVertices; ++i) {
    p[i] = Point3f(_p[i * 3 + 0], _p[i * 3 + 1], _p[i * 3 + 2]);
    n[i] = Normal3f(_n[i * 3 + 0], _n[i * 3 + 1], _n[i * 3 + 2]);
  }
  if (_uv.empty()) {
    DAKKU_WARN("no uv");
  } else {
    DAKKU_CHECK(_uv.size() == nVertices * 2, "wrong uv size");
    uv = std::make_unique<Point2f[]>(nVertices);
    for (int i = 0; i < nVertices; ++i) {
      uv[i] = Point2f(_uv[i * 2 + 0], _uv[i * 2 + 1]);
    }
  }
}

Interaction TriangleMesh::sample(const Point2f &u, Float &pdf) const {
  DAKKU_ERR("cannot call sample in triangle mesh, use triangle instead");
  return Interaction();
}

Float TriangleMesh::area() const {
  DAKKU_ERR("cannot call area in triangle mesh, use triangle instead");
  return 0;
}

void TriangleMeshPrimitive::unserialize(const Json &json, InputStream *stream) {
  std::string shapeName;
  if (json.contains("shape")) {
    json.at("shape").get_to(shapeName);
  } else {
    DAKKU_ERR("no shape for primitive");
  }
  const Shape *_shape{};
  if (auto it = renderState.shapes.find(shapeName);
      it != renderState.shapes.end()) {
    _shape = it->second.get();
  } else {
    DAKKU_ERR("cannot find shape: {}", shapeName);
  }

  std::string matName;
  if (json.contains("material")) {
    json.at("material").get_to(matName);
  } else {
    DAKKU_ERR("no material for primitive");
  }
  const Material *_material{};
  if (auto it = renderState.materials.find(matName);
      it != renderState.materials.end()) {
    _material = it->second.get();
  } else {
    DAKKU_ERR("cannot find material: {}", matName);
  }

  GeometricPrimitive::construct(_shape, _material, nullptr);
  const auto *triMesh = dynamic_cast<const TriangleMesh *>(this->shape);
  primTriangles =
      std::make_unique<GeometricPrimitive[]>(triMesh->getNumTriangles());
  for (int i = 0; i < triMesh->getNumTriangles(); ++i) {
    primTriangles[i].construct(triMesh->getTriangle(i), this->material,
                               this->areaLight);
  }
}

void TriangleMeshPrimitive::serialize(Json &json, OutputStream *stream) const {
  std::string shapeName;
  // TODO: optimize this
  for (const auto &[k, v] : renderState.shapes) {
    if (v.get() == this->shape) {
      shapeName = k;
      break;
    }
  }
  json["shape"] = shapeName;
}
DAKKU_END
