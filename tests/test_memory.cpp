// TODO: figure out how to make cmake run the damn tests

void test_read_file_into_memory() {
  auto mem = init_memory(MiB(64), MiB(48), TiB(2));

  const auto datafile = resources::locate_asset("somedata");
  auto npbytes = mem.persistent_bytes;
  auto ntbytes = mem.transient_bytes;
  const auto file1 = read_into_memory(datafile, &mem.persistent_ptr, &npbytes);
  const auto file2 = read_into_memory(datafile, &mem.transient_ptr, &ntbytes);
  spdlog::critical("delete this");

  // TODO: check memory state, check pointer locations
  //    (I did manually verify in the debugger ...)
}