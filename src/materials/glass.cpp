#include <materials/glass.h>
#include <core/interaction.h>
#include <core/reflection.h>
#include <core/memory.h>
#include <core/state.h>

DAKKU_BEGIN

void PerfectGlassMaterial::serialize(Json &json, OutputStream *stream) const {
  json["class"] = getClassNameStatic();
  std::string ktName;
  // TODO: optimize this
  for (const auto &[k, v] : renderState.spectrumTextures) {
    if (v.get() == kt) {
      ktName = k;
      break;
    }
  }
  json["kt"] = ktName;
  std::string krName;
  // TODO: optimize this
  for (const auto &[k, v] : renderState.spectrumTextures) {
    if (v.get() == kr) {
      krName = k;
      break;
    }
  }
  json["kr"] = krName;
  DAKKU_SER_J(etaI);
  DAKKU_SER_J(etaT);
}

void PerfectGlassMaterial::unserialize(const Json &json, InputStream *stream) {
  std::string ktName;
  if (!json.contains("kt")) {
    DAKKU_ERR("perfect glass material requires a kt!");
  } else {
    json.at("kt").get_to(ktName);
  }
  if (auto it = renderState.spectrumTextures.find(ktName);
      it != renderState.spectrumTextures.end()) {
    this->kt = it->second.get();
  } else {
    DAKKU_ERR("cannot find kt {} in texture map", ktName);
  }
  std::string krName;
  if (!json.contains("kr")) {
    DAKKU_ERR("perfect glass material requires a kr!");
  } else {
    json.at("kr").get_to(krName);
  }
  if (auto it = renderState.spectrumTextures.find(krName);
      it != renderState.spectrumTextures.end()) {
    this->kr = it->second.get();
  } else {
    DAKKU_ERR("cannot find kr {} in texture map", krName);
  }
  DAKKU_UNSER_JIE(etaI, 1);
  DAKKU_UNSER_JIE(etaT, 1);
}

void PerfectGlassMaterial::computeScatteringFunctions(
    SurfaceInteraction &si, MemoryArena &arena) const {
  si.bsdf = arena.allocObject<BSDF>(si);
  Spectrum r = kr->evaluate(si);
  Spectrum t = kt->evaluate(si);
  if (!t.isBlack()) {
    si.bsdf->add(arena.allocObject<FresnelSpecular>(r, t, etaI, etaT));
  }
}
DAKKU_END
