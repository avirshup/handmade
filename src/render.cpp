#include "render.h"
#include "./common.h"

typedef struct Pixel {
  // Note the order of the fields is weird because of little-endianness;
  // this makes it so IN MEMORY, as a uint32, it will be laid out (a, r, g, b).
  uint8 b, g, r, a;
} Pixel;

internal errcode resize_buffers(
    SDL_Renderer* renderer,
    SDL_Texture** texture,
    Pixel** bitmap_buff,
    const int w,
    const int h);

internal void paint_weird_gradient(Pixel* bitmap_buff, int w, int h, int t);

// continue https://davidgow.net/handmadepenguin/ch4.html

errcode paint_window(SDL_Window* window, const int t) {
  // TODO: store references in a struct, don't use static
  local_persistent SDL_Texture* texture;
  local_persistent Pixel* bitmap_buff;

  // get renderer, clear buffer
  SDL_Renderer* r = SDL_GetRenderer(window);
  CHECK_SDL_NOT_NULL(SDL_GetRenderer, r);
  CHECK_SDL_ERR(SDL_RenderClear(r));

  // alloc/resize buffers if necessary (todo: refactor into a struct probably)
  int w, h;
  SDL_GetWindowSize(window, &w, &h);
  if (resize_buffers(r, &texture, &bitmap_buff, w, h) != 0)
    return 1;

  // blit the texture
  // for these APIs, a null rect pointer means "the whole thing"
  paint_weird_gradient(bitmap_buff, w, h, t);
  CHECK_SDL_ERR(
      SDL_UpdateTexture(texture, nullptr, bitmap_buff, w * sizeof(Pixel)));
  CHECK_SDL_ERR(SDL_RenderCopy(r, texture, nullptr, nullptr));

  // Draw a rect border 5 px inside the viewport, why not
  const auto rect = SDL_Rect{.x = 5, .y = 5, .w = w - 10, .h = h - 10};
  CHECK_SDL_ERR(SDL_SetRenderDrawColor(r, 100, 0, 150, 255));
  CHECK_SDL_ERR(SDL_RenderDrawRect(r, &rect));

  // Set background color and draw, (I think?)
  CHECK_SDL_ERR(SDL_SetRenderDrawColor(r, 230, 230, 230, 255));
  SDL_RenderPresent(r);

  return 0;
}

internal void
paint_weird_gradient(Pixel* bitmap_buff, const int w, const int h, int t) {
  // Q: why looping _and_ incrementing the pointers at the same time???
  // A: because later the layout will be more complicated AIUI?
  Pixel* row = bitmap_buff;
  for (int y = 0; y < h; y++) {
    Pixel* pixel = row;
    for (int x = 0; x < w; x++) {
      *pixel = {
          .r = static_cast<uint8>(3 + x * t),
          .b = static_cast<uint8>(5 + y + 2 * t),
          .g = static_cast<uint8>(y + x + 13 + 3 * t)};
      ++pixel;
    }
    row += w;
  }
}

internal errcode resize_buffers(
    SDL_Renderer* renderer,
    SDL_Texture** texture,
    Pixel** bitmap_buff,
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

  if (*bitmap_buff != nullptr)
    free(*bitmap_buff);
  *bitmap_buff = static_cast<Pixel*>(malloc(w * h * sizeof(Pixel)));
  return 0;
}
