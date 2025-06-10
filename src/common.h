#ifndef COMMON_H
#define COMMON_H

#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>
#include <cassert>
#include <cstdint>
#include <iostream>

#define internalfn static
#define local_persistent static
#define global_var static

#define KiB(bytes) (1024LL * bytes)
#define MiB(bytes) (1024LL * KiB(bytes))
#define GiB(bytes) (1024LL * MiB(bytes))
#define TiB(bytes) (1024LL * GiB(bytes))

typedef int16_t Sample;

typedef int errcode;
typedef uint8_t uint8;
typedef Uint64 u64;
typedef Uint64 PCount;

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
