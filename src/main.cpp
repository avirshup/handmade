#include "./audio.h"
#include "./common.h"
#include "./events.h"
#include "./render.h"

// TODO: how to do 60fps if we only have ms tick resolution?
//  Choosing 16 or 17 is going to result in jitter or tearing or missed
//  frames.
//  TBH for lots of stuff (profiling timing etc) µs (or even
//  ns) might be a better resolution.
constexpr Uint64 MS_PER_FRAME = 16;  // 62.5 FPS
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

  /* ----- Init Audio ----- */
  init_audio();

  /* ----- Init graphics ----- */
  // init window
  SDL_Window* window = SDL_CreateWindow(
      "Handmade!",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      640,
      480,
      SDL_WINDOW_RESIZABLE);
  CHECK_SDL_NOT_NULL(SDL_CreateWindow, window);

  const SDL_Renderer* renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
  CHECK_SDL_NOT_NULL(SDL_CreateRenderer, renderer);

  /* ----- In The Loop ----- */
  auto next_tick = SDL_GetTicks64();
  auto tone_hz = 30;
  int frame_num = 0;

  while (true) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (handle_event(&event)) {
        spdlog::critical("Shutting down (event handler returned true)");
        return 0;
      }
    }

    // extremely hacky framerate cap, from
    // https://stackoverflow.com/a/75967125/1958900
    if (SDL_GetTicks64() > next_tick) {
      paint_window(window, frame_num++);

      if (queue_square_wave(tone_hz, 20) == 0) {
        // update tone
        tone_hz = tone_hz > 900 ? 30 : tone_hz + 1;
      };
      next_tick += MS_PER_FRAME;
    }
  }

  spdlog::error("Unexpected exit");
  return -1;
}
