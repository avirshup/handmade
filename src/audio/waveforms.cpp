#include "waveforms.h"
#include <math.h>
#include "constants.h"
using namespace std;

// FIXME state in global vars
global_var int PHASE = 0;
global_var Sample PHASE_SIGN = 1;

/**
 * Write a square wave of the requested duration into the passed buffer.
 * The vector will be resized to fit the data if necessary.
 */
size_t square_wave(
    vector<Sample>* buffer,
    const unsigned tone_hz,
    const unsigned duration_ms,
    const Sample volume) {
  const auto nsamples = duration_ms * SAMPLE_RATE_KHZ;
  const auto nbytes = nsamples * BYTES_PER_FRAME;
  const auto samples_per_half_period = SAMPLE_RATE_HZ / (2 * tone_hz);

  // TODO: will this ever *shrink* the vector's memory allocation?
  //    If so, is it possible to stop that from happening, so we
  //    just change the length?
  buffer->resize(NCHANNELS * nsamples);

  for (int idx = 0; idx < nsamples; ++idx) {
    PHASE++;
    if (PHASE % samples_per_half_period == 0) {
      PHASE = 0;
      PHASE_SIGN *= -1;
    }
    for (int ic = 0; ic < NCHANNELS; ++ic) {
      (*buffer)[NCHANNELS * idx + ic] = PHASE_SIGN * volume;
    }
  };

  return nbytes;
}

/**
 * Write a sine wave of the requested duration into the passed buffer.
 * The vector will be resized to fit the data if necessary.
 */
size_t sine_wave(
    vector<Sample>* buffer,
    const unsigned tone_hz,
    const unsigned duration_ms,
    const Sample volume) {
  const auto nsamples = duration_ms * SAMPLE_RATE_KHZ;
  const auto nbytes = nsamples * BYTES_PER_FRAME;
  const auto samples_per_half_period = SAMPLE_RATE_HZ / (2 * tone_hz);

  // TODO: will this ever *shrink* the vector's memory allocation?
  //    If so, is it possible to stop that from happening, so we
  //    just change the length?
  buffer->resize(NCHANNELS * nsamples);

  for (int idx = 0; idx < nsamples; ++idx) {
    PHASE++;
    if (PHASE % samples_per_half_period == 0) {
      // Q: why do you need to keep track of sign here?
      // A: so that if we switch to a square wave, it won't
      //    pop with a sign flip. ... Except:
      // TODO: this isn't at thing; the square and sine
      //     wave generators should have their own separate phases
      //     and then should be ramped and mixed to avoid pops
      PHASE = 0;
      PHASE_SIGN *= -1;
    }
    for (int ic = 0; ic < NCHANNELS; ++ic) {
      // TODO: check semantics for int <-> float conversions;
      //    I forgot to do any explicit conversions, but it seems to work?
      auto amp = volume * sin(PHASE * M_PI / samples_per_half_period);
      (*buffer)[NCHANNELS * idx + ic] = PHASE_SIGN * amp;
    }
  };

  return nbytes;
}
