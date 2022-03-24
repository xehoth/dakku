#ifndef DAKKU_SRC_CORE_RNG_H_
#define DAKKU_SRC_CORE_RNG_H_
#include <core/fwd.h>
#include <algorithm>

DAKKU_BEGIN

static constexpr std::uint64_t PCG32_DEFAULT_STATE = 0x853c49e6748fea9bull;
static constexpr std::uint64_t PCG32_DEFAULT_STREAM = 0xda3e39cb94b95bdbull;
static constexpr std::uint64_t PCG32_MULT = 0x5851f42d4c957f2dull;

// PCG32
class RNG {
 public:
  explicit RNG() = default;
  explicit RNG(std::uint64_t sequenceIndex) { setSequence(sequenceIndex); }
  inline void setSequence(std::uint64_t initseq);
  inline std::uint32_t uniformUInt32();
  std::uint32_t UniformUInt32(std::uint32_t b) {
    std::uint32_t threshold = (~b + 1u) % b;
    for (;;) {
      std::uint32_t r = uniformUInt32();
      if (r >= threshold) return r % b;
    }
  }
  Float uniformFloat() {
    return std::min(ONE_MINUS_EPSILON,
                    static_cast<Float>(uniformUInt32()) * 0x1p-32f);
  }

 private:
  std::uint64_t state{PCG32_DEFAULT_STATE}, inc{PCG32_DEFAULT_STREAM};
};

inline void RNG::setSequence(std::uint64_t initseq) {
  state = 0u;
  inc = (initseq << 1u) | 1u;
  uniformUInt32();
  state += PCG32_DEFAULT_STATE;
  uniformUInt32();
}

inline std::uint32_t RNG::uniformUInt32() {
  std::uint64_t oldstate = state;
  state = oldstate * PCG32_MULT + inc;
  auto xorshifted =
      static_cast<std::uint32_t>(((oldstate >> 18u) ^ oldstate) >> 27u);
  auto rot = static_cast<std::uint32_t>(oldstate >> 59u);
  return (xorshifted >> rot) | (xorshifted << ((~rot + 1u) & 31));
}
DAKKU_END
#endif  // DAKKU_SRC_CORE_RNG_H_
