#ifndef DAKKU_SRC_CORE_MATERIAL_H_
#define DAKKU_SRC_CORE_MATERIAL_H_
#include <core/sobject.h>

DAKKU_BEGIN
/**
 * dakku Material
 */
class Material : public SerializableObject {
 public:
  DAKKU_DECLARE_OBJECT(Material, SerializableObject);
  
  virtual void computeScatteringFunctions(SurfaceInteraction &si,
                                          MemoryArena &arena) const = 0;
};
DAKKU_END
#endif  // DAKKU_SRC_CORE_MATERIAL_H_
