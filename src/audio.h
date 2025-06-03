
#ifndef AUDIO_H
#define AUDIO_H

#include "./common.h"
#include "audio/constants.h"

errcode queue_audio(const void* data, size_t nbytes);
void pause_audio(bool pause);
errcode init_audio();
int queue_square_wave(float tone_hz, unsigned duration_ms);

#endif  // AUDIO_H
