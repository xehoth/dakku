#include <materials/metal.h>
#include <core/interaction.h>
#include <core/reflection.h>
#include <core/memory.h>
#include <core/state.h>

DAKKU_BEGIN
void MetalMaterial::computeScatteringFunctions(SurfaceInteraction &si,
                                               MemoryArena &arena) const {
  si.bsdf = arena.allocObject<BSDF>(si);
  Float uRough =
      uRoughness ? uRoughness->evaluate(si) : roughness->evaluate(si);
  Float vRough =
      vRoughness ? vRoughness->evaluate(si) : roughness->evaluate(si);
  Fresnel *fresnel = arena.allocObject<FresnelConductor>(
      Spectrum{1}, eta->evaluate(si), k->evaluate(si));
  MicrofacetDistribution *distribution =
      arena.allocObject<TrowbridgeReitzDistribution>(uRough, vRough);
  si.bsdf->add(arena.allocObject<MicrofacetReflection>(Spectrum{1},
                                                       distribution, fresnel));
}

void MetalMaterial::serialize(Json &json, OutputStream *stream) const {
  json["class"] = getClassNameStatic();
  DAKKU_ERR("unimplemented");
}

void MetalMaterial::unserialize(const Json &json, InputStream *stream) {
  if (json.contains("eta")) {
    std::string name;
    json.at("eta").get_to(name);
    if (auto it = renderState.spectrumTextures.find(name);
        it != renderState.spectrumTextures.end()) {
      this->eta = it->second.get();
    } else {
      DAKKU_ERR("cannot find eta {} in texture map", name);
    }
  } else {
    DAKKU_ERR("metal material requires eta texture");
  }
  if (json.contains("k")) {
    std::string name;
    json.at("k").get_to(name);
    if (auto it = renderState.spectrumTextures.find(name);
        it != renderState.spectrumTextures.end()) {
      this->k = it->second.get();
    } else {
      DAKKU_ERR("cannot find k {} in texture map", name);
    }
  } else {
    DAKKU_ERR("metal material requires k texture");
  }
  if (json.contains("uRoughness")) {
    std::string name;
    json.at("uRoughness").get_to(name);
    if (auto it = renderState.floatTextures.find(name);
        it != renderState.floatTextures.end()) {
      this->uRoughness = it->second.get();
    } else {
      DAKKU_ERR("cannot find uRoughness {} in texture map", name);
    }
  }
  if (json.contains("vRoughness")) {
    std::string name;
    json.at("vRoughness").get_to(name);
    if (auto it = renderState.floatTextures.find(name);
        it != renderState.floatTextures.end()) {
      this->vRoughness = it->second.get();
    } else {
      DAKKU_ERR("cannot find vRoughness {} in texture map", name);
    }
  }
  if (json.contains("roughness")) {
    std::string name;
    json.at("roughness").get_to(name);
    if (auto it = renderState.floatTextures.find(name);
        it != renderState.floatTextures.end()) {
      this->roughness = it->second.get();
    } else {
      DAKKU_ERR("cannot find roughness {} in texture map", name);
    }
  }
}

DAKKU_END