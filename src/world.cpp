#include "world.h"

using namespace glm;

// all accelerations are in pixels/s^2
constexpr float MAX_ACCEL = 800.0;
constexpr float GRAVITY = 400.0;

constexpr float DRAG = 1.0;  // ratio accel to velocity, units of 1/s

namespace world {
WorldState init_world() {
  return WorldState{.last_tick_ms = SDL_GetTicks64()};
}

/// External force (gravity, friction etc.)
vec2 external_acc(const vec2 pos, const vec2 vel) {
  return vec2{0.f, pos.y > 0. ? -GRAVITY : GRAVITY} - DRAG * vel;
}

void update_world(WorldState* world, const u64 tick_ms) {
  const float dt = static_cast<float>(tick_ms - world->last_tick_ms) / 1000.0f;

  // VV step 1 (old acceleration)
  const vec2 vhalf = world->vel + 0.5f * dt * world->last_acc;
  world->pos += vhalf * dt;

  // VV step 2 (new acceleration)
  // FIXME: user intent should be clamped to unit circle or less
  auto user_accel = MAX_ACCEL * world->user_intent;
  world->last_acc = user_accel + external_acc(world->pos, world->vel);
  world->vel += 0.5f * world->last_acc * dt;

  // advance timers
  world->last_tick_ms = tick_ms;
  world->time_s += dt;
}
}  // namespace world