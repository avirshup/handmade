#include "../audio.h"
#include "../common.h"
#include "constants.h"
#include "wave_generators.h"

using namespace std;

// FIXME state in global vars
global_var SDL_AudioDeviceID DEVICE_ID = 0;
global_var SDL_AudioSpec AUDIO_SPEC = {};  // <- actual reported spec after init
global_var vector<Sample> BUFFER = {};
global_var SineWaveGenerator sine_gen = SineWaveGenerator(1024, 0.5);
global_var SquareWaveGenerator square_gen = SquareWaveGenerator(1024, 0.5);

void pause_audio(const bool pause) {
  SDL_PauseAudioDevice(DEVICE_ID, pause ? 1 : 0);
}

/// Note the implementation here is actually generic over the IWaveGenerator
/// interface, just need to wire it up to accept one.
errcode queue_square_wave(const float tone_hz, const unsigned duration_ms) {
  if (SDL_GetQueuedAudioSize(DEVICE_ID) / BYTES_PER_MS + duration_ms >
      MAX_QUEUED_AUDIO_MS) {
    return 1;
  }

  if (duration_ms > 0) {
    spdlog::trace(
        "Will queue {} ms of {} hz square wave",
        duration_ms,
        tone_hz);

    // n.b., that's audio frames (48000 FPS), not video frames
    const auto num_frames = duration_ms * SAMPLE_RATE_KHZ;

    // TODO: does this ever _shrink_ the buffer's memory alloc?
    //  Ideally we keep it big even if the sample is small
    BUFFER.resize(NCHANNELS * num_frames);

    // note generator period is in audio frames
    const auto tone_period = static_cast<unsigned>(SAMPLE_RATE_HZ / tone_hz);
    square_gen.set_period(tone_period);

    for (int idx = 0; idx < num_frames; ++idx) {
      const auto amp = square_gen.next();
      for (int ic = 0; ic < NCHANNELS; ++ic) {
        BUFFER[NCHANNELS * idx + ic] = amp;
      }
    };

    const auto nbytes = num_frames * BYTES_PER_FRAME;
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
