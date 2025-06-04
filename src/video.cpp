#include "./video.h"
#include "./common.h"

/**** PRIVATE functions (public are below) ****/

internal void paint_weird_gradient(
    const WorldState* world,
    Pixel* bitmap_buff,
    const int w,
    const int h) {
  // Q: why looping _and_ incrementing the pointers at the same time???
  // A: because later the layout will be more complicated AIUI?
  Pixel* row = bitmap_buff;
  for (int y = 0; y < h; y++) {
    Pixel* pixel = row;
    for (int x = 0; x < w; x++) {
      const auto x_off = x - static_cast<int>(world->pos.x);
      const auto y_off = y - static_cast<int>(world->pos.y);
      *pixel = {
          .r = static_cast<uint8>(3 + x_off),
          .b = static_cast<uint8>(5 + y_off),
          .g = static_cast<uint8>(x_off + y_off + 13)};
      ++pixel;
    }
    row += w;
  }
}

internal errcode resize_buffer(
    SDL_Renderer* renderer,
    ScreenBuffer* buffer,
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

/**** PUBLIC ****/
VideoState init_video() {
  SDL_Window* window = SDL_CreateWindow(
      "Handmade!",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      640,
      480,
      SDL_WINDOW_RESIZABLE);
  if (window == nullptr) {
    throw std::runtime_error(
        "Failed to open window: " + std::string(SDL_GetError()));
  }

  SDL_Renderer* renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
  if (renderer == nullptr) {
    throw std::runtime_error(
        "Failed to create renderer: " + std::string(SDL_GetError()));
  }

  return VideoState{.window = window, .renderer = renderer};
}

errcode paint_window(VideoState* video, const WorldState* world) {
  // TODO: don't use C-style error handling here, probably?

  // get renderer, clear buffer
  CHECK_SDL_ERR(SDL_RenderClear(video->renderer));

  // alloc/resize buffers if necessary
  int w, h;
  SDL_GetWindowSize(video->window, &w, &h);

  ScreenBuffer* buffer = &video->screen_buffer;
  resize_buffer(video->renderer, buffer, w, h);

  // blit the texture
  // for these APIs, a null rect pointer means "the whole thing"
  paint_weird_gradient(world, buffer->pixel_buff, w, h);
  CHECK_SDL_ERR(SDL_UpdateTexture(
      buffer->texture,
      nullptr,
      buffer->pixel_buff,
      w * sizeof(Pixel)));
  CHECK_SDL_ERR(
      SDL_RenderCopy(video->renderer, buffer->texture, nullptr, nullptr));

  // Draw a rect border 5 px inside the viewport, why not
  const auto rect = SDL_Rect{.x = 5, .y = 5, .w = w - 10, .h = h - 10};
  CHECK_SDL_ERR(SDL_SetRenderDrawColor(video->renderer, 100, 0, 150, 255));
  CHECK_SDL_ERR(SDL_RenderDrawRect(video->renderer, &rect));

  // Set background color
  CHECK_SDL_ERR(SDL_SetRenderDrawColor(video->renderer, 230, 230, 230, 255));

  // draw it
  SDL_RenderPresent(video->renderer);

  return 0;
}
