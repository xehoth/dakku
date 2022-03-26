#ifndef DAKKU_SRC_MATERIALS_MATTE_H_
#define DAKKU_SRC_MATERIALS_MATTE_H_
#include <core/material.h>
#include <core/texture.h>

DAKKU_BEGIN
class MatteMaterial : public Material {
 public:
  DAKKU_DECLARE_OBJECT(MatteMaterial);
  void computeScatteringFunctions(SurfaceInteraction &si,
                                  MemoryArena &arena) const override;
  void serialize(Json &json, OutputStream *stream) const override;
  void unserialize(const Json &json, InputStream *stream) override;

 private:
  const Texture<Spectrum> *kd{};
};
DAKKU_END
#endif  // DAKKU_SRC_MATERIALS_MATTE_H_
