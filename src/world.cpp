#include "world.h"

WorldState init_world() {
  return WorldState{.last_tick_ms = SDL_GetTicks64()};
}

/// External force (gravity, friction etc.)
glm::vec2 external_acc(glm::vec2 pos, glm::vec2 vel) {
  return glm::vec2{0.f, pos.y > 0.f ? -1 : 1} - 0.1f * vel;
}

void update_world(WorldState* world, const u64 tick_ms) {
  const float dt = (tick_ms - world->last_tick_ms) / 1000.0;

  // VV step 1 (old acceleration)
  const glm::vec2 vhalf = world->vel + 0.5f * dt * world->last_acc;
  world->pos += vhalf * dt;

  // VV step 2 (new acceleration)
  world->last_acc = world->user_acc + external_acc(world->pos, world->vel);
  world->vel += world->vel + 0.5f * world->last_acc * dt;

  // advance timers
  world->last_tick_ms = tick_ms;
  world->time_s += dt;
}
