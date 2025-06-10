#ifndef RESOURCES_H
#define RESOURCES_H
#include <filesystem>
#include <string>
#include "./common.h"

using namespace std;

struct ReadFileResult {
  void* start_ptr;
  u64 size_bytes;
};

std::filesystem::path locate_asset(const std::string& subpath);
ReadFileResult
read_into_memory(filesystem::path path, void** mem_ptr, u64* max_size_bytes);
#endif  // RESOURCES_H