#ifndef DAKKU_SRC_MATERIALS_METAL_H_
#define DAKKU_SRC_MATERIALS_METAL_H_
#include <core/material.h>

DAKKU_BEGIN
class MetalMaterial : public Material {
 public:
  DAKKU_DECLARE_OBJECT(MetalMaterial, Material);
  void computeScatteringFunctions(SurfaceInteraction &si,
                                  MemoryArena &arena) const override;
  void serialize(Json &json, OutputStream *stream) const override;
  void unserialize(const Json &json, InputStream *stream) override;

 private:
  const Texture<Spectrum> *eta{}, *k{};
  const Texture<Float> *roughness{}, *uRoughness{}, *vRoughness{};
};
DAKKU_END
#endif  // DAKKU_SRC_MATERIALS_METAL_H_
