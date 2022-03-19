#include <shapes/triangle.h>
#include <core/logger.h>
#include <core/sampling.h>
#include <core/interaction.h>

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

void TriangleMesh::serialize(Json &json, OutputStream *stream) const {}
void TriangleMesh::unserialize(const Json &json, InputStream *stream) {}
DAKKU_END
