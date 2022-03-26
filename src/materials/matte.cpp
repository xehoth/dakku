#include <materials/matte.h>
#include <core/interaction.h>
#include <core/reflection.h>
#include <core/memory.h>
#include <core/state.h>

DAKKU_BEGIN

void MatteMaterial::computeScatteringFunctions(SurfaceInteraction &si,
                                               MemoryArena &arena) const {
  si.bsdf = arena.allocObject<BSDF>(si);
  Spectrum r = kd->evaluate(si);
  if (!r.isBlack()) {
    si.bsdf->add(arena.allocObject<LambertianReflection>(r));
  }
}

void MatteMaterial::serialize(Json &json, OutputStream *) const {
  json["class"] = getClassNameStatic();
  std::string texture;
  // TODO: optimize this
  for (const auto &[k, v] : renderState.spectrumTextures) {
    if (v.get() == kd) {
      texture = k;
      break;
    }
  }
  json["texture"] = texture;
}

void MatteMaterial::unserialize(const Json &json, InputStream *stream) {
  std::string texture;
  if (!json.contains("texture")) {
    DAKKU_ERR("matte material requires a texture!");
  } else {
    json.at("texture").get_to(texture);
  }
  if (auto it = renderState.spectrumTextures.find(texture);
      it != renderState.spectrumTextures.end()) {
    this->kd = it->second.get();
  } else {
    DAKKU_ERR("cannot find texture {} in texture map", texture);
  }
}

DAKKU_END
