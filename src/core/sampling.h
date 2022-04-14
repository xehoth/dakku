#ifndef DAKKU_CORE_SAMPLING_H_
#define DAKKU_CORE_SAMPLING_H_
#include <core/vector.h>

namespace dakku {

/**
 * @brief uniform sample a triangle
 *
 * @param u 2d random number
 * @return the barycentric coordinate of the sampled point
 */
Point2f uniformSampleTriangle(const Point2f &u);

/**
 * @brief sample disk
 *
 * @param u 2d random number
 */
Point2f concentricSampleDisk(const Point2f &u);

/**
 * @brief cosine weighted sample hemisphere
 *
 * @param u 2d random number
 * @return the sampled w in the hemisphere
 */
inline Vector3f cosineSampleHemisphere(const Point2f &u) {
  Point2f d = concentricSampleDisk(u);
  float z = std::sqrt(
      std::max(static_cast<float>(0), 1 - d.x() * d.x() - d.y() * d.y()));
  return Vector3f{d.x(), d.y(), z};
}

/**
 * @brief uniform sample a sphere
 *
 * @param u 2d random number
 * @return the sampled point
 */
Vector3f uniformSampleSphere(const Point2f &u);

inline float powerHeuristic(int nf, float fPdf, int ng, float gPdf) {
  float f = static_cast<float>(nf) * fPdf, g = static_cast<float>(ng) * gPdf;
  return (f * f) / (f * f + g * g);
}
}  // namespace dakku
#endif