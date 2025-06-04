#include "./perf.h"

PerfCounter init_perf() {
  return PerfCounter{.freq_hz = SDL_GetPerformanceFrequency()};
}

void perf_start(PerfCounter* counter, const PerfEvent event) {
  counter->starts[event] = SDL_GetPerformanceCounter();
  counter->ends.erase(event);
}

void perf_end(PerfCounter* counter, const PerfEvent event) {
  counter->ends[event] = SDL_GetPerformanceCounter();
}

constexpr float NO_EVENT = -2.0f;
constexpr float NOT_FINISHED = -1.0f;

/// Returns:
///  * timing in ms of the last event of this type, OR
///  * -2.0f if the event has never started,
///  * -1.0f if the event started but has not completed.
///  TODO: return a tagged enum / Result / Option instead of magic values
float last_perf_ms(const PerfCounter* counter, const PerfEvent event) {
  PCount start, end;
  if (const auto it = counter->starts.find(event);
      it != counter->starts.end()) {
    start = it->second;
  } else
    return NO_EVENT;

  if (const auto it = counter->ends.find(event); it != counter->ends.end()) {
    end = it->second;
  } else
    return NOT_FINISHED;

  return static_cast<float>(end - start) * 1000.f /
         static_cast<float>(counter->freq_hz);
}