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

Spectrum SurfaceInteraction::emit(const Vector3f &w) const {
  const AreaLight *areaLight = primitive->getAreaLight();
  return areaLight ? areaLight->emit(*this, w) : Spectrum(0);
}

void SurfaceInteraction::computeScatteringFunctions(const Ray &,
                                                    MemoryArena &arena) {
  primitive->computeScatteringFunctions(*this, arena);
}

DAKKU_END
