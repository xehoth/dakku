//
// Created by xehoth on 2021/10/2.
//
#include <dakku/shapes/triangle.h>
#include <dakku/core/transform.h>

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

std::pair<std::shared_ptr<TriangleMesh>,
          std::shared_ptr<std::vector<std::shared_ptr<Shape>>>>
createTriangleMesh(const Transform &o2w, const std::vector<int> &indices,
                   int nVertices, std::span<Point3f> pos,
                   std::span<Normal3f> normal, std::span<Point2f> texCoord) {
  std::shared_ptr<TriangleMesh> mesh = std::make_shared<TriangleMesh>(
      o2w, indices, nVertices, pos, normal, texCoord);
  auto tris = std::make_shared<std::vector<std::shared_ptr<Shape>>>();
  int nTriangles = static_cast<int>(indices.size()) / 3;
  tris->reserve(nTriangles);
  for (int i = 0; i < nTriangles; ++i) {
    tris->push_back(std::make_shared<Triangle>(mesh, i));
  }
  return std::make_pair(mesh, tris);
}
}  // namespace dakku
