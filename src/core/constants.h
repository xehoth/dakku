#ifndef DAKKU_CORE_CONSTANTS_H_
#define DAKKU_CORE_CONSTANTS_H_
#include <core/fwd.h>

#include <numbers>
#include <limits>

namespace dakku {

/// infinity
static constexpr float INF = std::numeric_limits<float>::infinity();

/// PI
static constexpr float PI = std::numbers::pi_v<float>;

/// $\frac 1 \pi$
static constexpr float INV_PI =
    static_cast<float>(1.0 / std::numbers::pi_v<double>);

/// $\frac \pi 2$
static constexpr float PI_OVER_2 =
    static_cast<float>(std::numbers::pi_v<double> / 2.0);

/// $\frac \pi 4$
static constexpr float PI_OVER_4 =
    static_cast<float>(std::numbers::pi_v<double> / 4.0);

/// shadow epsilon, used for shadow ray, offset ray origin
static constexpr float SHADOW_EPS = 1e-5f;

/// 1 - eps
static constexpr float ONE_MINUS_EPSILON =
    1 - std::numeric_limits<float>::epsilon();

}  // namespace dakku
#endif