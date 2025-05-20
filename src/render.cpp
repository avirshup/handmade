#include "render.h"
#include "./common.h"

bool paint_window(SDL_Window* window) {
  SDL_Renderer* r = SDL_GetRenderer(window);
  CHECK_SDL_NOT_NULL(SDL_GetRenderer, r);

  CHECK_SDL_ERR(SDL_RenderClear(r));

  int w, h;
  SDL_GetWindowSize(window, &w, &h);

  const auto rect = SDL_Rect{.x = 5, .y = 5, .w = w - 10, .h = h - 10};
  CHECK_SDL_ERR(SDL_SetRenderDrawColor(r, 100, 0, 150, 255));
  CHECK_SDL_ERR(SDL_RenderDrawRect(r, &rect));

  CHECK_SDL_ERR(SDL_SetRenderDrawColor(r, 230, 230, 230, 255));
  SDL_RenderPresent(r);
  return true;
}
