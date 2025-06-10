#include "../audio.h"
#include "../common.h"
#include "constants.h"
#include "wave_generators.h"

using namespace std;

// TODO: play more than 1 sound at once. Maybe: mix our own buffer
//   before queuing, use SDL_audiomixer, implement our own ring buffer?
//   DECISION: use a ring buffer. Why not SDL_Mixer? Because no more
//   dependencies. Why not mix before queuing? Because it's actually harder
//   than maintaining our ring buffer, PLUS it's more likely to give us
//   audio dropouts if stuff slows down for whatever reason, PLUS mixing
//   audio of different durations becomes intensely complicated.
//   see: https://ericscrivner.me/2017/10/getting-circular-sdl-audio/

// TODO: this will turn into our ring buffer
global_var vector<Sample> BUFFER = {};

void pause_audio(AudioState* state, const bool pause) {
  SDL_PauseAudioDevice(state->device_id, pause ? 1 : 0);
  state->paused = pause;
}

// TODO: how do we turn world state into sound?
//   Does the world_update directly talk to the audio system?
//   Or does the audio module take the world's state
//   and decide what to play based on it?
//   FOR NOW: no decision, just deal w/ it in the main loop

/// Note the implementation here is actually generic over the IWaveGenerator
/// interface, just need to wire it up to accept one.
errcode queue_square_wave(
    AudioState* state,
    const float tone_hz,
    const unsigned duration_ms) {
  if (SDL_GetQueuedAudioSize(state->device_id) / BYTES_PER_MS + duration_ms >
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
    state->square_gen1.set_period(tone_period);

    for (int idx = 0; idx < num_frames; ++idx) {
      const auto amp = state->square_gen1.next();
      for (int ic = 0; ic < NCHANNELS; ++ic) {
        BUFFER[NCHANNELS * idx + ic] = amp;
      }
    };

    if (const errcode err =
            queue_audio(state, BUFFER.data(), num_frames * BYTES_PER_FRAME);
        err != 0) {
      spdlog::error("Audio queuing issues: {}", SDL_GetError());
    }
  }
  return 0;
}

errcode queue_audio(AudioState* state, const void* data, const size_t nbytes) {
  CHECK_SDL_ERR(SDL_QueueAudio(state->device_id, data, nbytes));
  return 0;
}

AudioState init_audio() {
  constexpr SDL_AudioSpec spec = {
      .freq = SAMPLE_RATE_HZ,
      .format = AUDIO_FORMAT,
      .channels = NCHANNELS,
      .samples = NUM_SDL_AUDIO_BUFFER_FRAMES,
      .callback = nullptr};

  SDL_AudioSpec actual_spec = {};

  const auto device_id =
      SDL_OpenAudioDevice(nullptr, 0, &spec, &actual_spec, 0);

  printf(
      "Initialised Audio: %d Hz, %d Channels\n",
      actual_spec.freq,
      actual_spec.channels);

  assert(actual_spec.format == AUDIO_FORMAT);

  auto state = AudioState{.device_id = device_id, .audio_spec = actual_spec};

  // allocate our buffer now
  BUFFER.resize(AUDIO_BUFFER_SAMPLES);
  pause_audio(&state, false);

  return state;
}
