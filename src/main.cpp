#include "./audio.h"
#include "./common.h"
#include "./events.h"
#include "./memory.h"
#include "./state.h"
#include "./util.h"
#include "./video.h"
#include "./world.h"

// TODO: how to do 60fps if we only have ms tick resolution?
//  Choosing 16 or 17 is going to result in jitter or tearing or missed
//  frames. Maybe just use the performance counter? (on my machine,
//  it has ~40 ns resolution)
constexpr u64 MS_PER_FRAME = 16;  // 62.5 FPS
constexpr u64 MS_PER_TIMESTEP = 8;
// TODO: Why do I even have to manage these timers myself? Can
//    SDL somehow tell me when a refresh has happened? A counter
//    to tell me how many frames have actually displayed onscreen?
//    Maybe even a blocking call that releases after a VSYNC?
//  Here is SDL **3**'s API: https://wiki.libsdl.org/SDL3/SDL_SetRenderVSync

// SDL instead of windows: https://davidgow.net/handmadepenguin/ch1.html

// SDL docs (wiki???): https://wiki.libsdl.org/SDL2/SDL_CreateWindow

int main() {
  spdlog::set_level(spdlog::level::debug);

  CHECK_SDL_ERR(
      SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO));
  spdlog::critical("SDL initialized");

  auto perf_freq_hz = SDL_GetPerformanceFrequency();
  spdlog::debug("Perf freq: {}", perf_freq_hz);

#ifndef RELEASE
  spdlog::info("Running in DEV mode");
  void* base_address = reinterpret_cast<void*>(TiB(4));
#else
  void* base_address = nullptr;
#endif

  auto mem = init_memory(MiB(64), GiB(2), base_address);

  GameState state = {
      .video = init_video(),
      .audio = init_audio(),
      .input = init_input(),
      .perf = init_perf(),
      .world = init_world()};

  /* ----- Prep for the loop ----- */
  const auto first_tick = SDL_GetTicks64();
  auto next_draw_tick = first_tick;
  auto next_simulation_tick = first_tick;
  state.world.last_tick_ms = first_tick;

  /* ----- The loop ----- */
  SDL_Event event = {};
  while (true) {
    // 1) handle events
    while (SDL_PollEvent(&event)) {
      handle_event(&state, &event);
      if (state.should_quit) {
        spdlog::critical("Exit main loop (quit flag set)");
        return 0;
      }
    }

    const u64 tick = SDL_GetTicks64();

    if (tick >= next_simulation_tick) {
      update_world(&state.world, tick);
      next_simulation_tick = tick + MS_PER_TIMESTEP;
    }

    // Audio/Video runs here for now
    // (this is extremely hacky framerate limiter
    // https://stackoverflow.com/a/75967125/1958900)
    if (tick >= next_draw_tick) {
      MEASURE_PERF(AV, &state.perf, {
        MEASURE_PERF(
            PAINT, &state.perf, paint_window(&state.video, &state.world);)

        if (queue_square_wave(&state.audio, state.world.tone_hz, 20) == 0) {
          // update tone
          state.world.tone_hz =
              state.world.tone_hz > 900.f ? 30.f : state.world.tone_hz * 1.005f;
        };

        next_draw_tick = tick + MS_PER_FRAME;
        state.frame_num++;
      })

      if (state.frame_num % 120 == 0) {
        spdlog::debug(
            "Timings/ms for frame {}: paint:{:.3f}, "
            "total_update:{:.3f}",
            state.frame_num,
            last_perf_ms(&state.perf, PAINT),
            last_perf_ms(&state.perf, AV));
      }
    }
  }

  spdlog::error("Unexpected exit");
  return -1;
}
