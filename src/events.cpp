#include "./events.h"
#include "./common.h"
#include "./render.h"

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
          CHECK_SDL_NOT_NULL(SDL_GetWindowFromID, window);
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
