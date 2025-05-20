#include "render.h"
#include "./common.h"

static errcode resize_buffers(
    SDL_Renderer* renderer,
    SDL_Texture** texture,
    void** pixels,
    const int w,
    const int h);

// continue https://davidgow.net/handmadepenguin/ch4.html

errcode paint_window(SDL_Window* window) {
  // TODO: store references in a struct, don't use static
  local_persistent SDL_Texture* texture;
  local_persistent void* pixels;

  // get renderer, clear buffer
  SDL_Renderer* r = SDL_GetRenderer(window);
  CHECK_SDL_NOT_NULL(SDL_GetRenderer, r);
  CHECK_SDL_ERR(SDL_RenderClear(r));

  // alloc/resize buffers if necessary (todo: refactor into struct)
  int w, h;
  SDL_GetWindowSize(window, &w, &h);
  if (resize_buffers(r, &texture, &pixels, w, h) != 0)
    return 1;

  // blit the texture
  // nullptrs here are a synonym for "everything")
  CHECK_SDL_ERR(SDL_UpdateTexture(texture, nullptr, pixels, w * 4));
  CHECK_SDL_ERR(SDL_RenderCopy(r, texture, nullptr, nullptr));

  // Draw a rect border 5 px inside the viewport
  const auto rect = SDL_Rect{.x = 5, .y = 5, .w = w - 10, .h = h - 10};
  CHECK_SDL_ERR(SDL_SetRenderDrawColor(r, 100, 0, 150, 255));
  CHECK_SDL_ERR(SDL_RenderDrawRect(r, &rect));

  // Set background color and draw, (I think?)
  CHECK_SDL_ERR(SDL_SetRenderDrawColor(r, 230, 230, 230, 255));
  SDL_RenderPresent(r);

  return 0;
}

static errcode resize_buffers(
    SDL_Renderer* renderer,
    SDL_Texture** texture,
    void** pixels,
    const int w,
    const int h) {
  if (*texture != nullptr)
    SDL_DestroyTexture(*texture);

  // TODO: does texture retain a pointer to the renderer? [¯\_(ツ)_/¯]
  //   What is the renderer's lifetime? {  (/¯◡ ‿ ◡)/¯ ~ ┻━┻  }
  //   i miss rust ( ಥ╭╮ಥ )
  *texture = SDL_CreateTexture(
      renderer,
      SDL_PIXELFORMAT_ARGB8888,
      SDL_TEXTUREACCESS_STREAMING,
      w,
      h);
  CHECK_SDL_NOT_NULL(SDL_CreateTexture, *texture);

  if (*pixels != nullptr)
    free(*pixels);
  *pixels = malloc(4 * w * h);
  return 0;
}
