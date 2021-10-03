//
// Created by xehoth on 2021/10/1.
//

#ifndef DAKKU_INCLUDE_DAKKU_H_
#define DAKKU_INCLUDE_DAKKU_H_
#include <type_traits>
#include <limits>
#include <concepts>
#include <numbers>

namespace dakku {

#ifdef DAKKU_FLOAT_AS_DOUBLE
using Float = double;
#else
using Float = float;
#endif

constexpr Float INF = std::numeric_limits<Float>::infinity();
constexpr Float SHADOW_EPS = static_cast<Float>(1e-5);
constexpr Float PI = std::numbers::pi_v<Float>;
constexpr Float INV_PI = std::numbers::inv_pi_v<Float>;
constexpr Float PI_OVER2 = PI / 2;
constexpr Float PI_OVER4 = PI / 4;

class Scene;
class Integrator;
class SamplerIntegrator;
template <typename T>
requires std::is_arithmetic_v<T>
class Vector2;
template <typename T>
requires std::is_arithmetic_v<T>
class Vector3;
template <typename T>
requires std::is_arithmetic_v<T>
class Point2;
template <typename T>
requires std::is_arithmetic_v<T>
class Point3;
template <typename T>
requires std::is_arithmetic_v<T>
class Normal3;
class Ray;
class Transform;
struct Matrix4x4;
class RGBSpectrum;
class Film;
struct CameraSample;
class Camera;
class Shape;
struct Interaction;
class SurfaceInteraction;
class Primitive;
class Light;
class VisibilityTester;
class AreaLight;
class SceneParser;
class Sampler;
class Material;
class BxDF;
class BSDF;
}  // namespace dakku

#endif  // DAKKU_INCLUDE_DAKKU_H_
