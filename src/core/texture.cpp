//
// Created by xehoth on 2021/10/4.
//
#include <dakku/core/texture.h>
#include <dakku/core/interaction.h>

namespace dakku {

UVMapping2D::UVMapping2D(Float su, Float sv, Float du, Float dv)
    : su(su), sv(sv), du(du), dv(dv) {}

Point2f UVMapping2D::map(const SurfaceInteraction &si) const {
  return Point2f{su * si.uv.s + du, sv * si.uv.t + dv};
}

}  // namespace dakku