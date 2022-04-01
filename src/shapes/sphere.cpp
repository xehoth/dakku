#include <shapes/sphere.h>
#include <core/vector.h>
#include <core/interaction.h>
#include <core/sampling.h>
#include <core/transform_cache.h>
#include <core/fstream.h>
#include <span>

DAKKU_BEGIN

void Sphere::serialize(Json &json, OutputStream *) const {
  DAKKU_ERR("unimplemented");
}

void Sphere::unserialize(const Json &json, InputStream *stream) {
  DAKKU_UNSER_JI(o);
  DAKKU_UNSER_JI(radius);
  DAKKU_UNSER_JI(subdivisions);
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
  generateMesh();
  createTriangles(pToWorld, pToObject);
}

void Sphere::generateMesh() {
  const auto X = static_cast<Float>(0.525731112119133606);
  const auto Z = static_cast<Float>(0.850650808352039932);
  const Float N = 0;
  std::vector<Point3f> vertices{
      Point3f{-X, N, Z}, Point3f{X, N, Z},   Point3f{-X, N, -Z},
      Point3f{X, N, -Z}, Point3f{N, Z, X},   Point3f{N, Z, -X},
      Point3f{N, -Z, X}, Point3f{N, -Z, -X}, Point3f{Z, X, N},
      Point3f{-Z, X, N}, Point3f{Z, -X, N},  Point3f{-Z, -X, N}};
  std::vector<Vector3i> indices{
      Vector3i{0, 4, 1},  Vector3i{0, 9, 4},  Vector3i{9, 5, 4},
      Vector3i{4, 5, 8},  Vector3i{4, 8, 1},  Vector3i{8, 10, 1},
      Vector3i{8, 3, 10}, Vector3i{5, 3, 8},  Vector3i{5, 2, 3},
      Vector3i{2, 7, 3},  Vector3i{7, 10, 3}, Vector3i{7, 6, 10},
      Vector3i{7, 11, 6}, Vector3i{11, 0, 6}, Vector3i{0, 1, 6},
      Vector3i{6, 1, 10}, Vector3i{9, 0, 11}, Vector3i{9, 11, 2},
      Vector3i{9, 2, 5},  Vector3i{7, 2, 11}};
  auto subdivide =
      [](std::vector<Point3f> &vertices,
         const std::vector<Vector3i> &indices) -> std::vector<Vector3i> {
    std::vector<Vector3i> ret;
    std::map<std::pair<int, int>, int> map;
    auto midPoint = [&](int a, int b) {
      if (a > b) std::swap(a, b);
      std::pair<int, int> key(a, b);
      auto inserted = map.insert({key, static_cast<int>(vertices.size())});
      if (inserted.second)
        vertices.push_back((vertices[a] + vertices[b]).normalized());
      return inserted.first->second;
    };
    for (const auto &tri : indices) {
      int mid[3];
      for (int i = 0; i < 3; ++i) mid[i] = midPoint(tri[i], tri[(i + 1) % 3]);
      ret.emplace_back(tri[0], mid[0], mid[2]);
      ret.emplace_back(tri[1], mid[1], mid[0]);
      ret.emplace_back(tri[2], mid[2], mid[1]);
      ret.emplace_back(mid[0], mid[1], mid[2]);
    }
    return ret;
  };
  for (int i = 0; i < subdivisions; ++i) {
    indices = subdivide(vertices, indices);
  }
  std::vector<Normal3f> normals(vertices.size());
  for (size_t i = 0; i < normals.size(); ++i) {
    normals[i] = Normal3f(vertices[i]).normalized();
    vertices[i] = o + vertices[i] * (radius / vertices[i].norm());
  }
  assign(
      std::span<const int>{reinterpret_cast<const int *>(indices.data()),
                           indices.size() * 3},
      std::span<const Float>{reinterpret_cast<const Float *>(vertices.data()),
                             vertices.size() * 3},
      std::span<const Float>{reinterpret_cast<const Float *>(normals.data()),
                             normals.size() * 3},
      std::span<const Float>{});
}

Interaction Sphere::sample(const Point2f &u, Float &pdf) const {
  Point3f pObj = Point3f(0, 0, 0) + uniformSampleSphere(u);
  Interaction it;
  it.n = Normal3f(pObj).normalized();
  pObj *= radius / pObj.norm();
  it.p = o + pObj;
  pdf = 1 / area();
  return it;
}

Float Sphere::area() const { return 4 * PI * radius * radius; }

DAKKU_END
