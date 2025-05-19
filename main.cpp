#include <iostream>

#include "SDL.h"

// SDL instead of windows: https://davidgow.net/handmadepenguin/ch1.html

// SDL docs (wiki???): https://wiki.libsdl.org/SDL2/SDL_CreateWindow

bool paint_window(SDL_Window* w) {
  SDL_Renderer* r = SDL_GetRenderer(w);
  if (r == nullptr) {
    printf("Failed to get renderer ...");
    return false;
  }

  if (SDL_SetRenderDrawColor(r, 0, 255, 255, 255) != 0) {
    printf("render draw color: %s\n", SDL_GetError());
    return false;
  };

  if (SDL_RenderClear(r) != 0) {
    printf("render clear: %s\n", SDL_GetError());
    return false;
  };

  SDL_RenderPresent(r);
  return true;
}

// Event: https://wiki.libsdl.org/SDL2/SDL_Event
bool handle_event(const SDL_Event* event) {
  /* Returns true if it should quit */
  switch (event->type) {
    case SDL_QUIT: {
      printf("Got SDL_QUIT\n");
      return true;
    } break;

    case SDL_WINDOWEVENT: {
      switch (event->window.event) {
        case SDL_WINDOWEVENT_EXPOSED: {
          printf("should redraw\n");
          SDL_Window* window = SDL_GetWindowFromID(event->window.windowID);
          paint_window(window);
        } break;

        case SDL_WINDOWEVENT_RESIZED: {
          printf("Window resized.\n");
        } break;

        case SDL_WINDOWEVENT_CLOSE: {
          printf("Window closed, quit triggered\n");
          return true;
        } break;

        default: {
          // window event, whatever dude
        } break;
      }
    } break;

    default: {
      // printf("Unhandled: %d", event->type);
    } break;
  };

  return false;
}

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

  SDL_Renderer* _renderer = SDL_CreateRenderer(window, -1, 0);

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
