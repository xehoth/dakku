//
// Created by xehoth on 2021/10/2.
//
#include <dakku/accelerators/embree_accel.h>
#include <dakku/core/geometry.h>
#include <dakku/core/interaction.h>

namespace dakku {

EmbreeAccel::EmbreeAccel(std::shared_ptr<std::vector<TrianglePrimitivePair>> p)
    : p(p) {
  if (!this->rtcDevice) {
    this->rtcDevice = rtcNewDevice(nullptr);
  }
  if (!this->rtcScene) {
    this->rtcScene = rtcNewScene(this->rtcDevice);
    rtcSetSceneBuildQuality(this->rtcScene, RTC_BUILD_QUALITY_HIGH);
    rtcSetSceneFlags(this->rtcScene, RTC_SCENE_FLAG_ROBUST);
  }
  if (p) {
    int idx = 0;
    for (const auto &[mesh, prims] : *p) {
      auto geom = rtcNewGeometry(this->rtcDevice, RTC_GEOMETRY_TYPE_TRIANGLE);
      rtcSetSharedGeometryBuffer(geom, RTC_BUFFER_TYPE_VERTEX, 0,
                                 RTC_FORMAT_FLOAT3, mesh->p.data(), 0,
                                 sizeof(Point3f), mesh->nVertices);
      rtcSetSharedGeometryBuffer(geom, RTC_BUFFER_TYPE_INDEX, 0,
                                 RTC_FORMAT_UINT3, mesh->indices.data(), 0,
                                 sizeof(int) * 3, mesh->nTriangles);
      rtcCommitGeometry(geom);
      rtcAttachGeometryByID(this->rtcScene, geom, idx++);
      rtcReleaseGeometry(geom);
    }
  }
  rtcCommitScene(this->rtcScene);
}

RTCRay EmbreeAccel::toRtcRay(const Ray &ray) const {
  RTCRay ret{};
  ret.org_x = ray.o.x;
  ret.org_y = ray.o.y;
  ret.org_z = ray.o.z;
  ret.dir_x = ray.d.x;
  ret.dir_y = ray.d.y;
  ret.dir_z = ray.d.z;
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

  const auto &[mesh, prims] = (*this->p)[rayHit.hit.geomID];
  isect.primitive = (*prims)[rayHit.hit.primID];
  Float u = rayHit.hit.u;
  Float v = rayHit.hit.v;
  Float w = 1 - u - v;
  isect.p = r(rayHit.ray.tfar);
  int idx = rayHit.hit.primID * 3;
  isect.n = normalize(mesh->n[mesh->indices[idx + 0]] * w +
                      mesh->n[mesh->indices[idx + 1]] * u +
                      mesh->n[mesh->indices[idx + 2]] * v);
  r.tMax = rayHit.ray.tfar;

  return true;
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

bool EmbreeAccel::occluded(const Ray &r) const {
  RTCRay rtcRay = toRtcRay(r);
  RTCIntersectContext context{};
  rtcInitIntersectContext(&context);
  rtcOccluded1(this->rtcScene, &context, &rtcRay);
  return rtcRay.tfar < 0;
}
}  // namespace dakku