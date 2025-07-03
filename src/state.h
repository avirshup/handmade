#ifndef STATE_H
#define STATE_H

#include "./audio.h"
#include "./common.h"
#include "./inputs.h"
#include "./perf.h"
#include "./video.h"
#include "./world.h"

using namespace std;

namespace state {
struct GameState {
  u64 frame_num = 0;
  bool should_quit = false;

  video::VideoState video;
  inputs::InputState input;
  audio::AudioState audio;
  world::WorldState world;
  perf::PerfCounter perf;
};
}  // namespace state

#endif  // STATE_H
