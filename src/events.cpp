#include "./events.h"
#include "./common.h"
#include "./render.h"

// Event: https://wiki.libsdl.org/SDL2/SDL_Event
bool handle_event(const SDL_Event* event, const int t) {
  /* Returns true if it should quit */
  switch (event->type) {
    case SDL_QUIT: {
      printf("Got SDL_QUIT\n");
      return true;
    } break;

    case SDL_WINDOWEVENT: {
      // TODO: calls to paint window are not necessary here;
      //  we are repainting anyway, and the resize check allways happens
      //  anyway.
      // // go ahead and get the window
      // SDL_Window* window = SDL_GetWindowFromID(event->window.windowID);
      // // TODO: the errcode int this returns is not really compatible
      // //  with the bool this function returns:
      // CHECK_SDL_NOT_NULL(SDL_GetWindowFromID, window);)

      switch (event->window.event) {
        case SDL_WINDOWEVENT_EXPOSED: {
          printf("window exposed\n");
        } break;

        case SDL_WINDOWEVENT_RESIZED: {
          printf("Window resized.\n");
          // TODO: trigger resize here instead of checking while painting?
          // TODO: I may have missed a part about aspect ratios and stretching
        } break;

        case SDL_WINDOWEVENT_CLOSE: {
          printf("Window closed, should now quit\n");
          return true;
        } break;

        default: {
          // printf("Unhandled: %d", event->type);
        } break;
      }
    }

    default: {
      // printf("Unhandled: %d", event->type);
    } break;
  }
  return false;
}
