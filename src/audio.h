
#ifndef AUDIO_H
#define AUDIO_H

#include "./common.h"

errcode queue_audio(const void* data, int nbytes);
void pause_audio(bool pause);
errcode init_audio();
int play_square_wave(unsigned tone_hz, unsigned duration_ms);

#endif  // AUDIO_H
