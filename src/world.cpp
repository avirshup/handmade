#include "world.h"

void update_world(WorldState* world, const u64 tick_ms) {
  const double dt = (tick_ms - world->last_tick_ms) / 1000.0;

  world->last_tick_ms = tick_ms;
  world->time_s += dt;
}