//
// Created by xehoth on 2021/10/2.
//
#include <dakku/materials/matte.h>

namespace dakku {

MatteMaterial::MatteMaterial(const RGBSpectrum &Kd) : Kd(Kd) {}
void MatteMaterial::computeScatteringFunctions(SurfaceInteraction &si) const {

}
}