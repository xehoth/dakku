#ifndef DAKKU_CORE_MIPMAP_H_
#define DAKKU_CORE_MIPMAP_H_
#include <core/blocked_array.h>
#include <core/vector.h>
#include <core/spectrum.h>
#include <oneapi/tbb.h>
#include <utility>

namespace dakku {

/**
 * @brief image wrap mode
 *
 */
enum class ImageWrapMode { REPEAT, BLACK, CLAMP };

/**
 * @brief resample weight of the four nearby (continuous) texels
 *
 */
struct ResampleWeight {
  /// first texel index
  int firstTexel;
  /// resample weight of the four texels
  float weight[4];
};

/**
 * @brief mipmap class
 *
 * @tparam T data type
 */
template <typename T>
requires(std::is_same_v<T, float> || std::is_same_v<T, Spectrum>) class MipMap {
 public:
  explicit MipMap(Point2i resolution, std::span<const T> data,
                  bool doTrilinear = false, float maxAnisotropy = 8.0f,
                  ImageWrapMode wrapMode = ImageWrapMode::REPEAT);

  [[nodiscard]] int width() const { return resolution.x(); }
  [[nodiscard]] int height() const { return resolution.y(); }
  [[nodiscard]] size_t levels() const { return pyramid.size(); }

  /**
   * @brief get texel value
   *
   * @param level mipmap level
   */
  const T &texel(int level, int s, int t) const;

 private:
  /// do trilinear interpolation
  const bool doTrilinear;
  /// max anisotropy
  const float maxAnisotropy;
  /// image wrap mode
  const ImageWrapMode wrapMode;
  /// resolution
  Point2i resolution;
  /// mipmap pyramid
  std::vector<std::unique_ptr<BlockedArray<T>>> pyramid;

  /**
   * @brief get resampled weights
   *
   */
  std::unique_ptr<ResampleWeight[]> resampleWeights(int oldRes, int newRes) {
    DAKKU_CHECK(newRes >= oldRes, "invalid new resolution, expected: {} >= {}",
                newRes, oldRes);
    std::unique_ptr<ResampleWeight[]> wt =
        std::make_unique<ResampleWeight[]>(newRes);
    float filterWidth = 2.0f;
    for (int i = 0; i < newRes; ++i) {
      float center = (static_cast<float>(i) + 0.5f) /
                     static_cast<float>(newRes) * static_cast<float>(oldRes);
      wt[i].firstTexel = std::floor((center - filterWidth) * 0.5f);
      // nearby four texels
      for (int j = 0; j < 4; ++j) {
        float pos = static_cast<float>(wt[i].firstTexel + j) + 0.5f;
        wt[i].weight[j] = evalLanczos((pos - center) / filterWidth);
      }

      // normalize filter weights for texel resampling
      float inv = 1 / (wt[i].weight[0] + wt[i].weight[1] + wt[i].weight[2] +
                       wt[i].weight[3]);
      for (float &j : wt[i].weight) j *= inv;
    }
    return wt;
  }

  float clamp(float v) { return std::clamp(v, 0.0f, INF); }
  RgbSpectrum clamp(const RgbSpectrum &v) { return v.clamp(); }
};

template <typename T>
requires(std::is_same_v<T, float> ||
         std::is_same_v<T, Spectrum>) MipMap<T>::MipMap(Point2i resolution,
                                                        std::span<const T> img,
                                                        bool doTrilinear,
                                                        float maxAnisotropy,
                                                        ImageWrapMode wrapMode)
    : resolution(std::move(resolution)),
      doTrilinear(doTrilinear),
      maxAnisotropy(maxAnisotropy),
      wrapMode(wrapMode) {
  std::unique_ptr<T[]> resampledImage{};
  if (!isPowerOf2(resolution.x()) || !isPowerOf2(resolution.y())) {
    // not power of 2, resample image to the with the size to the nearest 2 ^ k
    Point2i resPow2(roundUpPow2(resolution.x()), roundUpPow2(resolution.y()));
    DAKKU_INFO("resampling mipmap from {} to {}", resolution, resPow2);
    std::unique_ptr<ResampleWeight[]> sWeights =
        resampleWeights(resolution.x(), resPow2.x());
    resampledImage = std::make_unique<T[]>(resPow2.x() * resPow2.y());

    // apply sWeights to zoom in $s$ direction
    oneapi::tbb::parallel_for(
        size_t{0}, static_cast<size_t>(resolution.y()), [&](size_t t) {
          size_t end = resPow2.x();
          for (size_t s = 0; s < end; ++s) {
            // compute texel $(s, t)$ in $s$-zoomed image
            resampledImage[t * resPow2[0] + s] = 0.f;
            for (int j = 0; j < 4; ++j) {
              int origS = sWeights[s].firstTexel + j;
              if (wrapMode == ImageWrapMode::REPEAT) {
                origS = origS % resolution[0];
              } else if (wrapMode == ImageWrapMode::CLAMP) {
                origS = std::clamp(origS, 0, resolution[0] - 1);
              }
              if (origS >= 0 && origS < (int)resolution[0]) {
                resampledImage[t * resPow2[0] + s] +=
                    sWeights[s].weight[j] * img[t * resolution[0] + origS];
              }
            }
          }
        });

    // resample image in $t$ direction
    std::unique_ptr<ResampleWeight[]> tWeights =
        resampleWeights(resolution.y(), resPow2.y());
    std::vector<T *> resampleBufs;
    int nThreads = oneapi::tbb::this_task_arena::max_concurrency();
    for (int i = 0; i < nThreads; ++i)
      resampleBufs.push_back(new T[resPow2[1]]);
    oneapi::tbb::parallel_for(
        size_t{0}, static_cast<size_t>(resolution.x()), [&](size_t s) {
          T *workData = resampleBufs
              [oneapi::tbb::this_task_arena::current_thread_index()];
          for (int t = 0; t < resPow2[1]; ++t) {
            workData[t] = 0.f;
            for (int j = 0; j < 4; ++j) {
              int offset = tWeights[t].firstTexel + j;
              if (wrapMode == ImageWrapMode::REPEAT) {
                offset = offset % resolution[1];
              } else if (wrapMode == ImageWrapMode::CLAMP) {
                offset = clamp(offset, 0, resolution[1] - 1);
              }
              if (offset >= 0 && offset < resolution[1]) {
                workData[t] += tWeights[t].weight[j] *
                               resampledImage[offset * resPow2[0] + s];
              }
            }
          }
          for (int t = 0; t < resPow2[1]; ++t)
            resampledImage[t * resPow2[0] + s] = clamp(workData[t]);
        });
    for (auto ptr : resampleBufs) delete[] ptr;
    resolution = resPow2;
  }
  // initialize levels of mipmap from image
  int nLevels = 1 + log2Int(std::max(resolution[0], resolution[1]));
  pyramid.resize(nLevels);

  // initialize the bottom level (most detailed)
  pyramid[0] = std::make_unique<BlockedArray<T>>(
      resolution[0], resolution[1],
      std::span<T>{resampledImage ? resampledImage.get() : img,
                   resolution.x() * resolution.y()});

  for (int i = 1; i < nLevels; ++i) {
    // next level: / 2
    int sRes = std::max(1, pyramid[i - 1]->uSize() / 2);
    int tRes = std::max(1, pyramid[i - 1]->vSize() / 2);
    pyramid[i] = std::make_unique<BlockedArray<T>>(sRes, tRes);

    // filter four texels from finer level of pyramid
    oneapi::tbb::parallel_for(0, tRes, [&](int t) {
      for (int s = 0; s < sRes; ++s) {
        (*pyramid[i])(s, t) = 0.25f * (texel(i - 1, 2 * s, 2 * t) +
                                       texel(i - 1, 2 * s + 1, 2 * t) +
                                       texel(i - 1, 2 * s, 2 * t + 1) +
                                       texel(i - 1, 2 * s + 1, 2 * t + 1));
      }
    });
  }

  // TODO: initialize EWA filter weights
}

template <typename T>
requires(std::is_same_v<T, float> ||
         std::is_same_v<T, Spectrum>) const T &MipMap<T>::texel(int level,
                                                                int s,
                                                                int t) const {
  DAKKU_CHECK(level < pyramid.size(), "level out of range: {} >= {}", level,
              pyramid.size());
  const BlockedArray<T> &l = *pyramid[level];
  switch (wrapMode) {
    case ImageWrapMode::REPEAT:
      s %= l.uSize();
      t %= l.vSize();
      break;
    case ImageWrapMode::CLAMP:
      s = std::clamp(s, 0, l.uSize() - 1);
      t = std::clamp(t, 0, l.vSize() - 1);
      break;
    case ImageWrapMode::BLACK: {
      static const T black{0.0f};
      if (s < 0 || s >= l.uSize() || t < 0 || t >= l.vSize()) return black;
      break;
    }
  }
  return l(s, t);
}
}  // namespace dakku
#endif