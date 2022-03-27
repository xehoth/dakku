#ifndef DAKKU_SRC_MATERIALS_GLASS_H_
#define DAKKU_SRC_MATERIALS_GLASS_H_
#include <core/material.h>

DAKKU_BEGIN
class PerfectGlassMaterial : public Material {
 public:
  DAKKU_DECLARE_OBJECT(PerfectGlassMaterial);
  void computeScatteringFunctions(SurfaceInteraction &si,
                                  MemoryArena &arena) const override;
  void serialize(Json &json, OutputStream *stream) const override;
  void unserialize(const Json &json, InputStream *stream) override;

 private:
  const Texture<Spectrum> *kr{};
  const Texture<Spectrum> *kt{};
  Float etaI{}, etaT{};
};
DAKKU_END
#endif  // DAKKU_SRC_MATERIALS_GLASS_H_
