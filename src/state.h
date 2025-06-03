
#ifndef STATE_H
#define STATE_H

#include "./audio.h"
#include "./common.h"
#include "./inputs.h"
#include "./video.h"
#include "./world.h"

using namespace std;

struct GameState {
  u64 frame_num = 0;
  bool should_quit = false;

  VideoState video;
  InputState input;
  AudioState audio;
  WorldState world;
};

#endif  // STATE_H
