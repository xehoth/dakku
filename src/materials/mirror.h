#ifndef DAKKU_SRC_MATERIALS_MIRROR_H_
#define DAKKU_SRC_MATERIALS_MIRROR_H_
#include <core/material.h>

DAKKU_BEGIN
class MirrorMaterial : public Material {
 public:
  DAKKU_DECLARE_OBJECT(MirrorMaterial, Material);
  void computeScatteringFunctions(SurfaceInteraction &si,
                                  MemoryArena &arena) const override;
  void serialize(Json &json, OutputStream *stream) const override;
  void unserialize(const Json &json, InputStream *stream) override;

 private:
  const Texture<Spectrum> *kr;
};
DAKKU_END
#endif  // DAKKU_SRC_MATERIALS_MIRROR_H_
