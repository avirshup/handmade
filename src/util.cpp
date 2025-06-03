
#include "util.h"

auto perf_to_ms(const u64 start, const u64 end, const u64 freq_hz) -> float {
  return static_cast<float>(end - start) * 1000.f / static_cast<float>(freq_hz);
}