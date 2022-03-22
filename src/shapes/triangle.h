#ifndef DAKKU_SRC_SHAPES_TRIANGLE_H_
#define DAKKU_SRC_SHAPES_TRIANGLE_H_
#include <core/shape.h>
#include <span>
#include <string>

DAKKU_BEGIN

class TriangleMesh;

class Triangle : public Shape {
 public:
  DAKKU_DECLARE_OBJECT(Triangle);

  explicit Triangle(const Transform *objectToWorld,
                    const Transform *worldToObject, const TriangleMesh *mesh,
                    int triNumber);

  void serialize(Json &json, OutputStream *stream) const override;
  void unserialize(const Json &json, InputStream *stream) override;
  [[nodiscard]] Float area() const override;
  Interaction sample(const Point2f &u, Float &pdf) const override;
  inline void getTexCoords(std::span<Point2f, 3> uv);

 private:
  const int *v{};
  const TriangleMesh *mesh{};
};

class TriangleMesh : public Shape {
 public:
  DAKKU_DECLARE_OBJECT(TriangleMesh);
  Interaction sample(const Point2f &u, Float &pdf) const override;
  [[nodiscard]] Float area() const override;
  void serialize(Json &json, OutputStream *stream) const override;
  void unserialize(const Json &json, InputStream *stream) override;

  friend class Triangle;

 private:
  void loadMesh(const std::string &path);
  void assign(std::span<const int> i, std::span<const Float> p,
              std::span<const Float> n, std::span<const Float> uv);
  int nTriangles{};
  int nVertices{};
  std::unique_ptr<int[]> indices{};
  std::unique_ptr<Point3f[]> p{};
  std::unique_ptr<Normal3f[]> n{};
  std::unique_ptr<Point2f[]> uv{};
  std::unique_ptr<Triangle[]> triangles{};
};

inline void Triangle::getTexCoords(std::span<Point2f, 3> uv) {
  if (mesh->uv) {
    uv[0] = mesh->uv[v[0]];
    uv[1] = mesh->uv[v[1]];
    uv[2] = mesh->uv[v[2]];
  } else {
    uv[0] = Point2f(0, 0);
    uv[1] = Point2f(1, 0);
    uv[2] = Point2f(1, 1);
  }
}

DAKKU_END
#endif  // DAKKU_SRC_SHAPES_TRIANGLE_H_
