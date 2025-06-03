#ifndef WORLD_H
#define WORLD_H

#include "common.h"

struct WorldState {
  u64 last_tick_ms = 0;
  double time_s = 0.0;
};

void update_world(WorldState* world, u64 tick_ms);
#endif  // WORLD_H
