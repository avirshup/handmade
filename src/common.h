#ifndef MACROS_H
#define MACROS_H

#include "SDL.h"

#include <iostream>

#define CHECK_SDL_ERR(expr)                                  \
  do {                                                       \
    if (expr != 0) {                                         \
      printf("ERROR: %s failed: %s", #expr, SDL_GetError()); \
      return false;                                          \
    }                                                        \
  } while (0)

#define CHECK_SDL_NOT_NULL(expr, val)                                       \
  do {                                                                      \
    if (val == nullptr) {                                                   \
      printf("ERROR: %s returned null pointer: %s", #expr, SDL_GetError()); \
      return false;                                                         \
    }                                                                       \
  } while (0)

#endif  // MACROS_H
