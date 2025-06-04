#include "./video.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "./common.h"
#include "./resources.h"

constexpr SDL_Color C_WHITE = {255, 255, 255, 255};
constexpr SDL_Color C_BLACK = {0, 0, 0, 255};

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
  // Initialize SDL_ttf
  if (TTF_Init() == -1) {
    throw std::runtime_error(
        "Failed to initialize SDL_ttf: " + std::string(TTF_GetError()));
  }

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

  // load font (TODO: does this belong here?)
  auto const debug_font_path = locate_asset("fonts/Hack-Regular.ttf");
  TTF_Font* debug_font = TTF_OpenFont(debug_font_path.c_str(), 12);
  if (debug_font == nullptr) {
    throw std::runtime_error(
        "Failed to load debug font: " + std::string(TTF_GetError()));
  }

  return VideoState{
      .window = window,
      .renderer = renderer,
      .debug_font = debug_font};
}

/// FIXME: LLM-generated code, needs inspection
errcode render_text(
    VideoState* video,
    const char* text,
    const int x,
    const int y,
    const SDL_Color color) {
  SDL_Surface* surface = TTF_RenderUTF8_Blended_Wrapped(
      video->debug_font,
      text,
      color,
      video->screen_buffer.w);
  if (surface == nullptr) {
    spdlog::error("Failed to render text: {}", TTF_GetError());
    return 1;
  }

  SDL_Texture* texture = SDL_CreateTextureFromSurface(video->renderer, surface);
  // TODO: do we really need to create and free this every time? Does it matter?
  SDL_FreeSurface(surface);

  if (texture == nullptr) {
    spdlog::error("Failed to create texture from surface: {}", SDL_GetError());
    return 1;
  }

  int w, h;
  SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
  SDL_Rect dest = {x, y, w, h};

  SDL_RenderCopy(video->renderer, texture, nullptr, &dest);
  SDL_DestroyTexture(texture);
  return 0;
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

  // Render debug text
  char debug_text[256];
  snprintf(
      debug_text,
      sizeof(debug_text),
      "Position: (%.1f, %.1f)\nVelocity: (%.1f, %.1f)\nAccel: (%.1f, "
      "%.1f)\nTime: %.1f",
      world->pos.x,
      world->pos.y,
      world->vel.x,
      world->vel.y,
      world->last_acc.x,
      world->last_acc.y,
      world->time_s);
  render_text(video, debug_text, 10, 10, C_WHITE);

  // draw it
  SDL_RenderPresent(video->renderer);

  return 0;
}
