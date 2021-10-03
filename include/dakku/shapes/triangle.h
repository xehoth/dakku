//
// Created by xehoth on 2021/10/2.
//

#ifndef DAKKU_INCLUDE_DAKKU_SHAPES_TRIANGLE_H_
#define DAKKU_INCLUDE_DAKKU_SHAPES_TRIANGLE_H_
#include <dakku/core/dakku.h>
#include <dakku/core/shape.h>
#include <dakku/core/geometry.h>
#include <span>
#include <vector>
#include <memory>

namespace dakku {

struct TriangleMesh {
  TriangleMesh(const Transform &objectToWorld, const std::vector<int> &indices,
               int nVertices, std::span<Point3f> pos,
               std::span<Normal3f> normal, std::span<Point2f> texCoord);

  const int nTriangles, nVertices;
  std::vector<int> indices;
  std::vector<Point3f> p;
  std::vector<Normal3f> n;
  std::vector<Point2f> uv;
};

class Triangle : public Shape {
 public:
  Triangle(const std::shared_ptr<TriangleMesh> &mesh, int triNumber);
  [[nodiscard]] Float area() const override;
  Interaction sample(const Interaction &ref, const Point2f &u,
                     Float &pdf) const override;

 private:
  std::shared_ptr<TriangleMesh> mesh{nullptr};
  std::span<const int, 3> v;
};

std::pair<std::shared_ptr<TriangleMesh>,
          std::shared_ptr<std::vector<std::shared_ptr<Shape>>>>
createTriangleMesh(const Transform &o2w, const std::vector<int> &indices,
                   int nVertices, std::span<Point3f> pos,
                   std::span<Normal3f> normal, std::span<Point2f> texCoord);
}  // namespace dakku
#endif  // DAKKU_INCLUDE_DAKKU_SHAPES_TRIANGLE_H_
