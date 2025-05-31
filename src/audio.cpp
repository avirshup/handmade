#include "./audio.h"
#include "./common.h"

using namespace std;

typedef int16_t Sample;

/***** Hardcoded audio settings *****/
/* Note "frame" here means an *audio* frame, i.e., 48000 "frames" per second */

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

/***** Stateful global vars :( *****/
global_var SDL_AudioDeviceID DEVICE_ID = 0;
global_var SDL_AudioSpec AUDIO_SPEC = {};  // <- actual reported spec after init

// square wave helpers
global_var int PHASE = 0;
global_var Sample PHASE_SIGN = 1;
global_var vector<Sample> BUFFER = {};

void pause_audio(const bool pause) {
  SDL_PauseAudioDevice(DEVICE_ID, pause ? 1 : 0);
}

errcode queue_audio(const void* data, const size_t nbytes) {
  CHECK_SDL_ERR(SDL_QueueAudio(DEVICE_ID, data, nbytes));
  return 0;
}

/**
 * Write a square wave of the requested duration into the passed buffer.
 * The vector will be resized to fit the data if necessary.
 */
internal size_t square_wave(
    vector<Sample>* buffer,
    const unsigned tone_hz,
    const unsigned duration_ms,
    const Sample volume) {
  const auto nsamples = duration_ms * SAMPLE_RATE_KHZ;
  const auto nbytes = nsamples * BYTES_PER_FRAME;
  const auto samples_per_half_period = SAMPLE_RATE_HZ / (2 * tone_hz);

  // TODO: will this ever *shrink* the vector's memory allocation?
  //    If so, is it possible to stop that from happening?
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

int play_square_wave(const unsigned tone_hz, const unsigned duration_ms) {
  if (SDL_GetQueuedAudioSize(DEVICE_ID) / BYTES_PER_MS + duration_ms >
      MAX_QUEUED_AUDIO_MS) {
    return 1;
  }

  if (duration_ms > 0) {
    spdlog::trace(
        "Will queue {} ms of {} hz square wave",
        duration_ms,
        tone_hz);

    const auto nbytes = square_wave(&BUFFER, tone_hz, duration_ms, 2000);
    if (const errcode err = queue_audio(BUFFER.data(), nbytes); err != 0) {
      spdlog::error("Audio queuing issues: {}", SDL_GetError());
    }
  }
  return 0;
}

errcode init_audio() {
  constexpr SDL_AudioSpec spec = {
      .freq = SAMPLE_RATE_HZ,
      .format = AUDIO_FORMAT,
      .channels = NCHANNELS,
      .samples = NUM_SDL_AUDIO_BUFFER_FRAMES,
      .callback = nullptr};

  DEVICE_ID = SDL_OpenAudioDevice(nullptr, 0, &spec, &AUDIO_SPEC, 0);

  printf(
      "Initialised Audio: %d Hz, %d Channels\n",
      AUDIO_SPEC.freq,
      AUDIO_SPEC.channels);

  assert(AUDIO_SPEC.format == AUDIO_FORMAT);

  // allocate our buffer now
  BUFFER.resize(AUDIO_BUFFER_SAMPLES);
  pause_audio(false);

  return 0;
}
