#include "../audio.h"
#include "../common.h"
#include "constants.h"
#include "waveforms.h"

using namespace std;

// FIXME state in global vars
global_var SDL_AudioDeviceID DEVICE_ID = 0;
global_var SDL_AudioSpec AUDIO_SPEC = {};  // <- actual reported spec after init
global_var vector<Sample> BUFFER = {};

void pause_audio(const bool pause) {
  SDL_PauseAudioDevice(DEVICE_ID, pause ? 1 : 0);
}

errcode queue_square_wave(const unsigned tone_hz, const unsigned duration_ms) {
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

errcode queue_audio(const void* data, const size_t nbytes) {
  CHECK_SDL_ERR(SDL_QueueAudio(DEVICE_ID, data, nbytes));
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
