//
// Created by xehoth on 2021/10/2.
//
#include <dakku/shapes/triangle.h>
#include <dakku/core/transform.h>
#include <dakku/core/interaction.h>
#include <dakku/core/geometry.h>
#include <dakku/core/sampling.h>

namespace dakku {

TriangleMesh::TriangleMesh(const Transform &objectToWorld,
                           const std::vector<int> &indices, int nVertices,
                           std::span<Point3f> pos, std::span<Normal3f> normal,
                           std::span<Point2f> texCoord)
    : nTriangles(static_cast<int>(indices.size() / 3)), nVertices(nVertices) {
  this->indices = indices;
  p.resize(nVertices);
  for (int i = 0; i < nVertices; ++i) p[i] = objectToWorld(pos[i]);
  if (!normal.empty()) {
    n.resize(nVertices);
    for (int i = 0; i < nVertices; ++i) n[i] = objectToWorld(normal[i]);
  }
  if (!texCoord.empty()) {
    uv.assign(texCoord.begin(), texCoord.begin() + nVertices);
  }
}

Triangle::Triangle(const std::shared_ptr<TriangleMesh> &mesh, int triNumber)
    : Shape(),
      mesh(mesh),
      v(mesh->indices.begin() + 3 * triNumber,
        mesh->indices.begin() + 3 * triNumber + 3) {}

Float Triangle::area() const {
  const Point3f &p0 = mesh->p[v[0]];
  const Point3f &p1 = mesh->p[v[1]];
  const Point3f &p2 = mesh->p[v[2]];
  return cross(p1 - p0, p2 - p0).length() / 2;
}

Interaction Triangle::sample(const Point2f &u, Float &pdf) const {
  Point2f b = uniformSampleTriangle(u);
  const Point3f &p0 = mesh->p[v[0]];
  const Point3f &p1 = mesh->p[v[1]];
  const Point3f &p2 = mesh->p[v[2]];
  Interaction it;
  it.p = p0 * b.x + p1 * b.y + p2 * (1 - b.x - b.y);
  it.n = normalize(Normal3f{cross(p1 - p0, p2 - p0)});
  if (!mesh->n.empty()) {
    Normal3f ns(mesh->n[v[0]] * b.x + mesh->n[v[1]] * b.y +
                mesh->n[v[2]] * (1 - b.x - b.y));
    it.n = faceforward(it.n, ns);
  }

  pdf = 1 / area();
  return it;
}

std::shared_ptr<TriangleMeshShapes> createTriangleMesh(
    const Transform &o2w, const std::vector<int> &indices, int nVertices,
    std::span<Point3f> pos, std::span<Normal3f> normal,
    std::span<Point2f> texCoord) {
  auto ret = std::make_shared<TriangleMeshShapes>();
  ret->mesh = std::make_shared<TriangleMesh>(o2w, indices, nVertices, pos,
                                             normal, texCoord);
  const int nTriangles = static_cast<int>(indices.size()) / 3;
  ret->shapes.reserve(nTriangles);
  for (int i = 0; i < nTriangles; ++i) {
    ret->shapes.push_back(std::make_shared<Triangle>(ret->mesh, i));
  }
  return ret;
}
}  // namespace dakku
