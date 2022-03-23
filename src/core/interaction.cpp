#include <core/interaction.h>

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

DAKKU_END
