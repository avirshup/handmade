#ifndef COMMON_H
#define COMMON_H

#include <cassert>
#include <cstdint>
#include <iostream>
#include "SDL.h"
#include "spdlog/spdlog.h"

#define internal static
#define local_persistent static
#define global_var static

typedef int16_t Sample;

typedef int errcode;
typedef uint8_t uint8;
typedef Uint64 u64;

#define CHECK_SDL_ERR(expr)                                         \
  do {                                                              \
    if (expr != 0) {                                                \
      spdlog::error("ERROR: {} failed: {}", #expr, SDL_GetError()); \
      return 1;                                                     \
    }                                                               \
  } while (0)  // why wrapped in a while(0)? ... reasons.

#define CHECK_SDL_NOT_NULL(expr, val)            \
  do {                                           \
    if (val == nullptr) {                        \
      spdlog::error(                             \
          "ERROR: {} returned null pointer: {}", \
          #expr,                                 \
          SDL_GetError());                       \
      return 1;                                  \
    }                                            \
  } while (0)

#endif  // COMMON_H
