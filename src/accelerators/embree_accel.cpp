#include <accelerators/embree_accel.h>
#include <core/ray.h>
#include <core/interaction.h>

DAKKU_BEGIN
EmbreeAccel::EmbreeAccel(std::span<const Primitive *> _primitives) {
  primitives.resize(_primitives.size());
  triangles.resize(primitives.size());
  for (size_t i = 0; i < _primitives.size(); ++i) {
    const auto *op = _primitives[i];
    DAKKU_CHECK(
        op->getClassName() == TriangleMeshPrimitive::getClassNameStatic(),
        "embree accel only supports triangle mesh");
    const auto *p = dynamic_cast<const TriangleMeshPrimitive *>(op);
    DAKKU_CHECK(
        p->getShape()->getClassName() == TriangleMesh::getClassNameStatic(),
        "embree accel only supports triangle mesh");
    const auto *tri = dynamic_cast<const TriangleMesh *>(p->getShape());
    primitives[i] = p;
    triangles[i] = tri;
  }
  if (!this->rtcDevice) this->rtcDevice = rtcNewDevice(nullptr);
  if (!this->rtcScene) {
    this->rtcScene = rtcNewScene(this->rtcDevice);
    rtcSetSceneBuildQuality(this->rtcScene, RTC_BUILD_QUALITY_HIGH);
    rtcSetSceneFlags(this->rtcScene, RTC_SCENE_FLAG_ROBUST);
  }
  int idx = 0;
  for (const auto &tri : triangles) {
    auto geom = rtcNewGeometry(this->rtcDevice, RTC_GEOMETRY_TYPE_TRIANGLE);
    rtcSetSharedGeometryBuffer(geom, RTC_BUFFER_TYPE_VERTEX, 0,
                               RTC_FORMAT_FLOAT3, tri->getVertices(), 0,
                               sizeof(Point3f), tri->getNumVertices());
    rtcSetSharedGeometryBuffer(geom, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3,
                               tri->getIndices(), 0, sizeof(int) * 3,
                               tri->getNumTriangles());
    rtcCommitGeometry(geom);
    rtcAttachGeometryByID(this->rtcScene, geom, idx++);
    rtcReleaseGeometry(geom);
  }
  rtcCommitScene(this->rtcScene);
}

EmbreeAccel::~EmbreeAccel() {
  if (this->rtcScene) {
    rtcReleaseScene(this->rtcScene);
    this->rtcScene = nullptr;
  }
  if (this->rtcDevice) {
    rtcReleaseDevice(this->rtcDevice);
    this->rtcDevice = nullptr;
  }
}

inline RTCRay toRtcRay(const Ray &ray) {
  RTCRay ret{};
  ret.org_x = ray.o.x();
  ret.org_y = ray.o.y();
  ret.org_z = ray.o.z();
  ret.dir_x = ray.d.x();
  ret.dir_y = ray.d.y();
  ret.dir_z = ray.d.z();
  ret.tnear = 0;
  ret.tfar = ray.tMax;
  ret.flags = 0;
  return ret;
}

bool EmbreeAccel::intersect(const Ray &r, SurfaceInteraction &isect) const {
  RTCRayHit rayHit{};
  rayHit.ray = toRtcRay(r);
  rayHit.hit.geomID = RTC_INVALID_GEOMETRY_ID;
  rayHit.hit.primID = RTC_INVALID_GEOMETRY_ID;
  rayHit.hit.instID[0] = RTC_INVALID_GEOMETRY_ID;

  RTCIntersectContext context{};
  rtcInitIntersectContext(&context);
  rtcIntersect1(rtcScene, &context, &rayHit);
  if (rayHit.hit.geomID == RTC_INVALID_GEOMETRY_ID ||
      rayHit.hit.primID == RTC_INVALID_GEOMETRY_ID)
    return false;

  const TriangleMeshPrimitive *p = this->primitives[rayHit.hit.geomID];
  const GeometricPrimitive *triP = p->getTrianglePrimitive(rayHit.hit.primID);
  const TriangleMesh *mesh = this->triangles[rayHit.hit.geomID];

  Point3f pHit = r(rayHit.ray.tfar);

  Float u = rayHit.hit.u;
  Float v = rayHit.hit.v;
  const Triangle *tri = mesh->getTriangle(rayHit.hit.primID);
  Point2f uvs[3];
  tri->getTexCoords(uvs);
  Point2f uv = barycentricInterpolate(uvs[1], uvs[2], uvs[0], Point2f(u, v));
  Normal3f geomNormal(rayHit.hit.Ng_x, rayHit.hit.Ng_y, rayHit.hit.Ng_z);

  isect = SurfaceInteraction(pHit, uv, -r.d, geomNormal, tri);
  Normal3f hitN = tri->getShadingNormal(u, v);
  isect.setShadingGeometry(hitN, true);
  isect.primitive = triP;
  return false;
}

bool EmbreeAccel::intersectP(const Ray &r) const {
  RTCRay rtcRay = toRtcRay(r);
  RTCIntersectContext context{};
  rtcInitIntersectContext(&context);
  rtcOccluded1(this->rtcScene, &context, &rtcRay);
  return rtcRay.tfar < 0;
}

void EmbreeAccel::serialize(Json &json, OutputStream *) const {
  DAKKU_ERR("unimplemented");
}
void EmbreeAccel::unserialize(const Json &json, InputStream *) {
  DAKKU_ERR("unimplemented");
}

DAKKU_END