#include "./render.h"
#include "./common.h"

// continue https://davidgow.net/handmadepenguin/ch6.html

typedef struct Pixel {
  // Note the order of the fields is weird because of little-endianness;
  // IN MEMORY, as a uint32, it will be laid out (a, r, g, b).
  uint8 b, g, r, a;
} Pixel;

typedef struct ScreenBuffer {
  SDL_Texture* texture;
  Pixel* pixel_buff;
  int w;
  int h;
} Buffer;

internal errcode
resize_buffer(SDL_Renderer* renderer, Buffer* buffer, int w, int h);
internal void paint_weird_gradient(Pixel* bitmap_buff, int w, int h, int t);

errcode paint_window(SDL_Window* window, const int t) {
  // TODO: pass this in, don't store state with statics
  local_persistent Buffer buffer;

  // get renderer, clear buffer
  SDL_Renderer* r = SDL_GetRenderer(window);
  CHECK_SDL_NOT_NULL(SDL_GetRenderer, r);
  CHECK_SDL_ERR(SDL_RenderClear(r));

  // alloc/resize buffers if necessary (todo: refactor into a struct probably)
  int w, h;
  SDL_GetWindowSize(window, &w, &h);
  resize_buffer(r, &buffer, w, h);

  // blit the texture
  // for these APIs, a null rect pointer means "the whole thing"
  paint_weird_gradient(buffer.pixel_buff, w, h, t);
  CHECK_SDL_ERR(SDL_UpdateTexture(
      buffer.texture,
      nullptr,
      buffer.pixel_buff,
      w * sizeof(Pixel)));
  CHECK_SDL_ERR(SDL_RenderCopy(r, buffer.texture, nullptr, nullptr));

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
          .r = static_cast<uint8>(3 + x + t),
          .b = static_cast<uint8>(5 + y + 2 * t),
          .g = static_cast<uint8>(y + x + 13 + 3 * t)};
      ++pixel;
    }
    row += w;
  }
}

internal errcode resize_buffer(
    SDL_Renderer* renderer,
    Buffer* buffer,
    const int w,
    const int h) {
  if (buffer->w == w && buffer->h == h)
    return 0;

  if (buffer->texture != nullptr)
    SDL_DestroyTexture(buffer->texture);
  if (buffer->pixel_buff != nullptr)
    delete buffer->pixel_buff;

  // TODO: does texture retain a pointer to the renderer? [¯\_(ツ)_/¯]
  //   What is the renderer's lifetime? {  (/¯◡ ‿ ◡)/¯ ~ ┻━┻  }
  //   i miss rust ( ಥ╭╮ಥ )
  *buffer = {
      .texture = SDL_CreateTexture(
          renderer,
          SDL_PIXELFORMAT_ARGB8888,
          SDL_TEXTUREACCESS_STREAMING,
          w,
          h),
      .pixel_buff = new Pixel[w * h],
      .w = w,
      .h = h};

  CHECK_SDL_NOT_NULL(SDL_CreateTexture, buffer->texture);

  return 0;
}
