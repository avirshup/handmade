#include "./audio.h"
#include "./common.h"
#include "./events.h"
#include "./render.h"

constexpr Uint64 MS_PER_FRAME =
    17;  // approx. 60 fps (FIXME: find a better way)

// SDL instead of windows: https://davidgow.net/handmadepenguin/ch1.html

// SDL docs (wiki???): https://wiki.libsdl.org/SDL2/SDL_CreateWindow
int main() {
  CHECK_SDL_ERR(
      SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO));

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
  int frame_num = 0;

  while (true) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (handle_event(&event)) {
        printf("DEBUG: event triggered shutdown");
        return 0;
      }
    }

    // extremely hacky framerate cap, from
    // https://stackoverflow.com/a/75967125/1958900
    if (SDL_GetTicks64() > next_tick) {
      paint_window(window, frame_num++);
      play_square_wave();
      next_tick += MS_PER_FRAME;
    }
  }
}
