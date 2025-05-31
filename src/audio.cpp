#include "./audio.h"
#include "./common.h"

using namespace std;

// More state in global vars!
global_var SDL_AudioDeviceID DEVICE_ID = 0;
global_var SDL_AudioSpec AUDIO_SPEC = {};  // actual spec being used
global_var int RUNNING_INDEX = 0;
global_var vector<int16_t> BUFFER = {};

// signed, 16-bit, little endian, see
// https://wiki.libsdl.org/SDL2/SDL_AudioFormat
constexpr SDL_AudioFormat AUDIO_FORMAT = AUDIO_S16LSB;
constexpr int SAMPLE_RATE_HZ = 48000;
constexpr int NCHANNELS = 2;
constexpr Uint16 AUDIO_CHANNEL_BUFFER = 2048;
constexpr Uint32 BYTES_PER_SAMPLE = sizeof(Uint16) * NCHANNELS;

void pause_audio(const bool pause) {
  SDL_PauseAudioDevice(DEVICE_ID, pause ? 1 : 0);
}

errcode queue_audio(const void* data, const int nbytes) {
  CHECK_SDL_ERR(SDL_QueueAudio(DEVICE_ID, data, nbytes));
  return 0;
}

internal int square_wave(
    vector<int16_t>* buffer,
    const int tone_hz,
    const int duration_ms,
    const int16_t volume) {
  const int nsamples = duration_ms * (SAMPLE_RATE_HZ / 1000);
  const int nbytes = nsamples * BYTES_PER_SAMPLE;
  const int samples_per_half_period = SAMPLE_RATE_HZ / (2 * tone_hz);

  // extend buffer if necessary. RN we only grow it, never shrink it.
  if (buffer->size() < nsamples) {
    buffer->resize(NCHANNELS * nsamples);
  }

  int phase = 0;
  auto amplitude = volume;
  for (int idx = 0; idx < nsamples; ++idx) {
    for (int ic = 0; ic < NCHANNELS; ++ic) {
      (*buffer)[NCHANNELS * idx + ic] = amplitude;
    }
    phase++;
    if (phase % samples_per_half_period == 0) {
      phase = 0;
      amplitude *= -1;
    }
  };

  return nbytes;
}

void play_square_wave() {
  vector<int16_t> buffer = {};

  // TODO: keep an upper bound on how big the queue gets, this will fill it up
  // to infinity
  const auto nbytes = square_wave(&buffer, 1000, 17, 2000);
  errcode err = queue_audio(buffer.data(), nbytes);
  if (err != 0) {
    printf("Uh oh %s", SDL_GetError());
  }
  // printf("Queued audio: %d\n", SDL_GetQueuedAudioSize(DEVICE_ID));
}

errcode init_audio() {
  constexpr SDL_AudioSpec spec = {
      .freq = SAMPLE_RATE_HZ,
      .format = AUDIO_FORMAT,
      .channels = NCHANNELS,
      .samples =
          SAMPLE_RATE_HZ * BYTES_PER_SAMPLE / 60,  // umm, what is this exactly?
      .callback = nullptr};

  DEVICE_ID = SDL_OpenAudioDevice(nullptr, 0, &spec, &AUDIO_SPEC, 0);

  printf(
      "Initialised Audio: %d Hz, %d Channels\n",
      AUDIO_SPEC.freq,
      AUDIO_SPEC.channels);

  assert(AUDIO_SPEC.format == AUDIO_FORMAT);
  pause_audio(false);

  return 0;
}
