#ifndef RENDER_H
#define RENDER_H

#include "./common.h"
#include "./world.h"

typedef struct Pixel {
  // Note the order of the fields is weird because of little-endianness;
  // IN MEMORY, as a uint32, it will be laid out (a, r, g, b).
  uint8 b, g, r, a;
} Pixel;

struct ScreenBuffer {
  SDL_Texture* texture;
  Pixel* pixel_buff;
  int w;
  int h;
};

struct VideoState {
  SDL_Window* window;
  SDL_Renderer* renderer;
  ScreenBuffer screen_buffer;
};

VideoState init_video();
errcode paint_window(VideoState* video, const WorldState* state);

#endif  // RENDER_H
