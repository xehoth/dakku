#ifndef DAKKU_SRC_SHAPES_TRIANGLE_H_
#define DAKKU_SRC_SHAPES_TRIANGLE_H_
#include <core/shape.h>
#include <core/primitive.h>
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
  inline void getTexCoords(std::span<Point2f, 3> uv) const;
  [[nodiscard]] inline Normal3f getShadingNormal(Float _u, Float _v) const;
  void computeInteraction(Float b0, Float b1, Float b2,
                          SurfaceInteraction &isect) const;

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
  [[nodiscard]] const int *getIndices() const { return indices.get(); }
  [[nodiscard]] const Point3f *getVertices() const { return p.get(); }
  [[nodiscard]] int getNumTriangles() const { return nTriangles; }
  [[nodiscard]] int getNumVertices() const { return nVertices; }
  [[nodiscard]] const Triangle *getTriangle(size_t idx) const {
    return &triangles[idx];
  }
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

inline void Triangle::getTexCoords(std::span<Point2f, 3> uv) const {
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

Normal3f Triangle::getShadingNormal(Float _u, Float _v) const {
  return Normal3f((1 - _u - _v) * Vector3f(mesh->n[v[0]]) +
                  _u * Vector3f(mesh->n[v[1]]) + _v * Vector3f(mesh->n[v[2]]))
      .normalized();
}

class TriangleMeshPrimitive : public GeometricPrimitive {
 public:
  DAKKU_DECLARE_OBJECT(TriangleMeshPrimitive);

  void serialize(Json &json, OutputStream *stream) const override;
  void unserialize(const Json &json, InputStream *stream) override;

  [[nodiscard]] const GeometricPrimitive *getTrianglePrimitive(
      size_t idx) const {
    return &primTriangles[idx];
  }

  std::unique_ptr<GeometricPrimitive[]> primTriangles;
};

DAKKU_END
#endif  // DAKKU_SRC_SHAPES_TRIANGLE_H_
