#include "./common.h"
#include "./events.h"

// SDL instead of windows: https://davidgow.net/handmadepenguin/ch1.html

// SDL docs (wiki???): https://wiki.libsdl.org/SDL2/SDL_CreateWindow
int main() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("FATAL: SDL init failed\n");
    return 1;
  }
  SDL_Window* window = SDL_CreateWindow(
      "Handmade!",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      640,
      480,
      SDL_WINDOW_RESIZABLE);
  CHECK_SDL_NOT_NULL(SDL_CreateWindow, window);

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
  CHECK_SDL_NOT_NULL(SDL_CreateRenderer, renderer);

  // if (SDL_ShowSimpleMessageBox(
  //         SDL_MESSAGEBOX_WARNING,
  //         "Handmade Hero",
  //         "It's running!",
  //         0) != 0) {
  //   printf("FATAL: msg box failed\n");
  //   return 1;
  // }

  // the main loop I guess?
  for (;;) {
    SDL_Event event;
    SDL_WaitEvent(&event);
    if (handle_event(&event)) {
      printf("DEBUG: event triggered shutdown");
      break;
    }
  }

  return 0;
}
