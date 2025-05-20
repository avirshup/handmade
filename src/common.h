#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include "SDL.h"

#define internal static
#define local_persistent static
#define global_var static
#define errcode int

#define CHECK_SDL_ERR(expr)                                  \
  do {                                                       \
    if (expr != 0) {                                         \
      printf("ERROR: %s failed: %s", #expr, SDL_GetError()); \
      return 1;                                              \
    }                                                        \
  } while (0)  // why wrapped in a while(0)? ¯\_(ツ)_/¯

#define CHECK_SDL_NOT_NULL(expr, val)                                       \
  do {                                                                      \
    if (val == nullptr) {                                                   \
      printf("ERROR: %s returned null pointer: %s", #expr, SDL_GetError()); \
      return 1;                                                             \
    }                                                                       \
  } while (0)

#endif  // COMMON_H
