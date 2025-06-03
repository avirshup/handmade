#ifndef AUDIO_H
#define AUDIO_H

#include "./audio/wave_generators.h"
#include "./common.h"

struct AudioState {
  bool paused = false;

  SDL_AudioDeviceID device_id = 0;
  SDL_AudioSpec audio_spec = {};

  SineWaveGenerator sine_gen1 = SineWaveGenerator(1024, 0.5);
  SquareWaveGenerator square_gen1 = SquareWaveGenerator(1024, 0.5);
};

errcode queue_audio(AudioState* state, const void* data, size_t nbytes);
void pause_audio(AudioState* state, bool pause);
AudioState init_audio();
int queue_square_wave(AudioState* state, float tone_hz, unsigned duration_ms);

#endif  // AUDIO_H
