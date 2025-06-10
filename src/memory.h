
#ifndef MEMORY_H
#define MEMORY_H
#include "./common.h"

struct GameMemory {
  u64 persistent_bytes;
  void* persistent_start;
  void* persistent_ptr;

  u64 transient_bytes;
  void* transient_start;
  void* transient_ptr;
};

GameMemory init_memory(
    const u64 persistent_bytes,
    const u64 transient_bytes,
    void* base_address);

#endif  // MEMORY_H
