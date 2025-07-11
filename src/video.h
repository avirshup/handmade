#ifndef VIDEO_H
#define VIDEO_H

#include <SDL2/SDL_ttf.h>
#include "./common.h"
#include "./world.h"

namespace video {

struct Pixel {
  // Note the order of the fields is weird because of little-endianness;
  // IN MEMORY, as a uint32, it will be laid out (a, r, g, b).
  uint8 b, g, r, a;
};

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
  TTF_Font* debug_font;
};

VideoState init_video();
errcode paint_window(VideoState* video, const world::WorldState* state);
errcode
render_text(VideoState* video, const char* text, int x, int y, SDL_Color color);

}  // namespace video

#endif  // VIDEO_H
