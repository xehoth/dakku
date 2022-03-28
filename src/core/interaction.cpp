#include <core/interaction.h>
#include <core/primitive.h>
#include <core/spectrum.h>
#include <core/light.h>

DAKKU_BEGIN

void SurfaceInteraction::setShadingGeometry(const Normal3f &_n,
                                            bool orientationIsAuthoritative) {
  shading.n = _n;
  if (orientationIsAuthoritative) {
    n = n.faceForward(shading.n);
  } else {
    shading.n = shading.n.faceForward(n);
  }
}

void SurfaceInteraction::setShadingGeometry(const Vector3f &_dpdu,
                                            const Vector3f &_dpdv,
                                            const Normal3f &_dndu,
                                            const Normal3f &_dndv,
                                            bool orientationIsAuthoritative) {
  shading.n = Normal3f(_dpdu.cross(_dpdv));
  if (orientationIsAuthoritative) {
    n = n.faceForward(shading.n);
  } else {
    shading.n = shading.n.faceForward(n);
  }
  shading.dpdu = _dpdu;
  shading.dpdv = _dpdv;
  shading.dndu = _dndu;
  shading.dndv = _dndv;
}

Spectrum SurfaceInteraction::emit(const Vector3f &w) const {
  const AreaLight *areaLight = primitive->getAreaLight();
  return areaLight ? areaLight->emit(*this, w) : Spectrum(0);
}

void SurfaceInteraction::computeScatteringFunctions(const Ray &,
                                                    MemoryArena &arena) {
  primitive->computeScatteringFunctions(*this, arena);
}

DAKKU_END
