
#ifndef PERF_H
#define PERF_H

#include <unordered_map>
#include "common.h"
using namespace std;

enum PerfEvent {
  PAINT,
  AV,
  SIM,
};

/// Barebones perf counter, which could be later used to track things
/// like rolling averages and frametime distributions.
struct PerfCounter {
  PCount freq_hz;
  unordered_map<PerfEvent, PCount> starts;
  unordered_map<PerfEvent, PCount> ends;
};

PerfCounter init_perf();
void perf_start(PerfCounter* counter, const PerfEvent event);
void perf_end(PerfCounter* counter, const PerfEvent event);
float last_perf_ms(const PerfCounter* counter, const PerfEvent event);

#define MEASURE_PERF(name, counter, block) \
  perf_start(counter, name);               \
  block perf_end(counter, name);

#endif  // PERF_H
