#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "../common.h"

/***** Hardcoded audio settings *****/
/* Note "frame" here means an *audio* frame, i.e., 48000 "frames" per second */

typedef int16_t Sample;

// signed, 16-bit, little endian -- https://wiki.libsdl.org/SDL2/SDL_AudioFormat
constexpr SDL_AudioFormat AUDIO_FORMAT = AUDIO_S16LSB;
constexpr auto SAMPLE_RATE_KHZ = 48;
constexpr auto NCHANNELS = 2;
constexpr auto NUM_SDL_AUDIO_BUFFER_FRAMES = 2048;
constexpr auto MAX_QUEUED_AUDIO_MS = 10000;

// Our queue buffer: 2^18 samples (~5.5s of 48kHz 16-bit stereo)
constexpr auto AUDIO_BUFFER_SAMPLES = 262144;

/***** Some helpful quantities *****/
constexpr auto BYTES_PER_FRAME = sizeof(Sample) * NCHANNELS;
constexpr auto AUDIO_BUFFER_BYTES = AUDIO_BUFFER_SAMPLES * BYTES_PER_FRAME;
constexpr int SAMPLE_RATE_HZ = SAMPLE_RATE_KHZ * 1000;
constexpr auto BYTES_PER_MS = BYTES_PER_FRAME * SAMPLE_RATE_KHZ;
constexpr auto AUDIO_BUFFER_MS = AUDIO_BUFFER_BYTES / BYTES_PER_MS;

#endif  // CONSTANTS_H
