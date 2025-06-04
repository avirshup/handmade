#ifndef WORLD_H
#define WORLD_H

#include "common.h"
#include "glm/vec2.hpp"

struct WorldState {
  u64 last_tick_ms = 0;
  float time_s = 0.0;
  float tone_hz = 30.0;
  glm::vec2 pos{0.0f, 0.0f};
  glm::vec2 vel{0.0f, 0.0f};
  glm::vec2 user_acc{};
  glm::vec2 last_acc{};
};

WorldState init_world();

void update_world(WorldState* world, u64 tick_ms);
#endif  // WORLD_H
