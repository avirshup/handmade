
#ifndef AUDIO_H
#define AUDIO_H

#include "./common.h"

errcode queue_audio(const void* data, const int nbytes);
void pause_audio(const bool pause);
errcode init_audio();
void play_square_wave();

#endif  // AUDIO_H
