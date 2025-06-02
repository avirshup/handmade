#ifndef WAVEFORMS_H
#define WAVEFORMS_H

#include "../common.h"
#include "constants.h"

size_t square_wave(
    std::vector<Sample>* buffer,
    unsigned tone_hz,
    unsigned duration_ms,
    Sample volume);

size_t sine_wave(
    std::vector<Sample>* buffer,
    unsigned tone_hz,
    unsigned duration_ms,
    Sample volume);
#endif  // WAVEFORMS_H
