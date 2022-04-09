#ifndef DAKKU_CORE_BLOCKED_ARRAY_H_
#define DAKKU_CORE_BLOCKED_ARRAY_H_
#include <core/fwd.h>
#include <span>

namespace dakku {

/**
 * @brief blocked array
 * $s \times s$ elements are layout together (linear), where $s$ is the block
 * size, then block by block linear layout
 * block size $s$ is power of $2$
 * This improves the performance for looking up nearby elements (for texture)
 *
 * @tparam T data type
 * @tparam logBlockSize $\log s$ where $s$ is the block size
 */
template <typename T, int logBlockSize>
class BlockedArray {
 public:
  /**
   * @brief Construct a new Blocked Array object
   *
   * @param uRes u resolution (width)
   * @param vRes v resolution (height)
   * @param d data
   */
  explicit BlockedArray(int uRes, int vRes, std::span<const T> d = {})
      : uRes(uRes), vRes(vRes) {
    int nAlloc = roundUp(uRes) * roundUp(vRes);
    data = new (std::max(alignof(T), std::align_val_t(L1_CACHE_LINE_SIZE)))
        T[nAlloc]();
    if (!d.empty()) {
      for (int v = 0; v < vRes; ++v)
        for (int u = 0; u < uRes; ++u) (*this)(u, v) = d[v * uRes + u];
    }
  }

  ~BlockedArray() {
    delete[]((std::max(alignof(T), std::align_val_t(L1_CACHE_LINE_SIZE))),
             data);
  }

  /**
   * @brief get block idx for a index
   *
   * @param i the lookup index
   * @return (i / block size)
   */
  [[nodiscard]] int blockIdx(int i) const { return i >> logBlockSize; }

  /**
   * @brief get the offset within a block for a lookup index
   *
   * @param i the lookup index
   * @return (i % block size)
   */
  [[nodiscard]] int offset(int i) const { return i & (blockSize() - 1); }

  /**
   * @brief get block size
   *
   */
  [[nodiscard]] constexpr int blockSize() const { return 1 << logBlockSize; }

  /**
   * @brief the round up size, the smallest multiples of block size >= x
   *
   *
   * @param x the requested size
   * @return the round up size
   */
  [[nodiscard]] int roundUp(int x) const {
    return (x + blockSize() - 1) & ~(blockSize() - 1);
  }

  [[nodiscard]] int uSize() const { return uRes; }
  [[nodiscard]] int vSize() const { return vRes; }

  /**
   * @brief lookup data
   *
   */
  const T &operator()(int u, int v) const {
    int bu = blockIdx(u), bv = blockIdx(v);
    int ou = offset(u), ov = offset(v);
    int off =
        blockSize() * blockSize() * (uBlocks * bv + bu) + blockSize() * ov + ou;
    return data[off];
  }

  /**
   * @brief lookup data
   *
   */
  T &operator()(int u, int v) {
    return const_cast<T &>(static_cast<const BlockedArray &>(*this)(u, v));
  }

  /**
   * @brief to linear array
   *
   */
  void toLinearArray(std::span<T> a) {
    int idx = 0;
    for (int v = 0; v < vRes; ++v)
      for (int u = 0; u < uRes; ++u) a[idx++] = (*this)(u, v);
  }

 private:
  /// data
  T *data;
  /// u resolution
  const int uRes;
  /// v resolution
  const int vRes;
  /// u blocks
  const int uBlocks;
};
}  // namespace dakku
#endif