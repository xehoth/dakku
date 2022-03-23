#include <shapes/triangle.h>
#include <core/logger.h>
#include <core/sampling.h>
#include <core/interaction.h>
#include <core/stream.h>
#include <core/transform_cache.h>
#include <core/relative.h>
#include <core/state.h>
#include <igl/readOBJ.h>
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

  it.p = barycentricInterpolate(p0, p1, p2, b);
  // compute surface normal for sampled point on triangle
  it.n = Normal3f((p1 - p0).cross(p2 - p0)).normalized();
  // ensure correct orientation of the geometric normal
  if (mesh->n) {
    Normal3f ns =
        barycentricInterpolate(mesh->n[v[0]], mesh->n[v[1]], mesh->n[v[2]], b);
    it.n = faceforward(it.n, ns);
  } else if (transformSwapsHandedness) {
    it.n = -it.n;
  }
  pdf = 1 / area();
  return it;
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
