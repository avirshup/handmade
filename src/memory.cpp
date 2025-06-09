#include "memory.h"
#include <sys/mman.h>
#include "./common.h"

GameMemory init_memory(
    const u64 persistent_bytes,
    const u64 transient_bytes,
    void* base_address) {
  void* mem_block = mmap(
      base_address,
      persistent_bytes + transient_bytes,
      PROT_READ | PROT_WRITE,
      MAP_ANON | MAP_PRIVATE,
      -1,
      0);

  assert(mem_block != nullptr);  // ensure it allocated

  // Q: why uint8* here?
  // A: because we want to advance the pointer by 8 bits per byte :)
  void* transient_ptr = static_cast<uint8*>(mem_block) + persistent_bytes;

  return GameMemory{
      .persistent_bytes = persistent_bytes,
      .persistent = mem_block,

      .transient_bytes = transient_bytes,
      .transient = transient_ptr,
  };
}