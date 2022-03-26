#include <materials/mirror.h>
#include <core/interaction.h>
#include <core/reflection.h>
#include <core/memory.h>
#include <core/state.h>

DAKKU_BEGIN

void MirrorMaterial::computeScatteringFunctions(SurfaceInteraction &si,
                                                MemoryArena &arena) const {
  si.bsdf = arena.allocObject<BSDF>(si);
  Spectrum r = kr->evaluate(si);
  if (!r.isBlack()) {
    si.bsdf->add(arena.allocObject<SpecularReflection>(
        r, arena.allocObject<FresnelNoOperation>()));
  }
}

void MirrorMaterial::serialize(Json &json, OutputStream *stream) const {
  json["class"] = getClassNameStatic();
  std::string texture;
  // TODO: optimize this
  for (const auto &[k, v] : renderState.spectrumTextures) {
    if (v.get() == kr) {
      texture = k;
      break;
    }
  }
  json["texture"] = texture;
}

void MirrorMaterial::unserialize(const Json &json, InputStream *stream) {
  std::string texture;
  if (!json.contains("texture")) {
    DAKKU_ERR("mirror material requires a texture!");
  } else {
    json.at("texture").get_to(texture);
  }
  if (auto it = renderState.spectrumTextures.find(texture);
      it != renderState.spectrumTextures.end()) {
    this->kr = it->second.get();
  } else {
    DAKKU_ERR("cannot find texture {} in texture map", texture);
  }
}

DAKKU_END
