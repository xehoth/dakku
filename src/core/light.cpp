//
// Created by xehoth on 2021/10/2.
//
#include <dakku/core/light.h>

namespace dakku {

Light::Light(LightFlags flags) : flags(flags) {}

AreaLight::AreaLight() : Light(LightFlags::AREA) {}
}