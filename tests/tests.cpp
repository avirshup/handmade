#include <gtest/gtest.h>
#include "../src/memory.h"
#include "../src/resources.h"

TEST(AllocTests, TestAllocMemory) {
  auto mem =
      memory::init_memory(MiB(64), MiB(48), reinterpret_cast<void*>(TiB(2)));

  const auto datafile = resources::locate_asset("somedata");
  auto npbytes = mem.persistent_bytes;
  auto ntbytes = mem.transient_bytes;
  const auto file1 =
      resources::read_into_memory(datafile, &mem.persistent_ptr, &npbytes);
  const auto file2 =
      resources::read_into_memory(datafile, &mem.transient_ptr, &ntbytes);

  // TODO: check memory state, check pointer locations
  //    (I did manually verify in the debugger ...)
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}