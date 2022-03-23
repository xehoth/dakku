#ifndef DAKKU_SRC_CORE_INTEGRATOR_H_
#define DAKKU_SRC_CORE_INTEGRATOR_H_
#include <core/sobject.h>

DAKKU_BEGIN
class Integrator : public SerializableObject {
 public:
  virtual void render(const Scene &scene) = 0;
};

//Spectrum uniformSampleOneLight(const Interaction &it, const Scene &scene, MemoryArena &arena, )
DAKKU_END
#endif  // DAKKU_SRC_CORE_INTEGRATOR_H_
