#include "./resources.h"
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include "./common.h"

using namespace std;

filesystem::path locate_asset(const std::string& subpath) {
  // ASSET_PATH is defined in CMakeLists.txt
  filesystem::path asset_root(ASSET_PATH);
  filesystem::path full_path = absolute((asset_root / subpath));

  if (!filesystem::exists(full_path)) {
    throw runtime_error("Asset not found: " + full_path.string());
  }

  spdlog::debug("Located: {}", full_path.string());

  return full_path;
}

/// Read contents of file into memory starting at mem_ptr.
/// Advances mem_ptr to the end of the newly written memory and
/// reduces the `max_size` counter by the same amount
/// returns ReadFileResult that points to the written memory
///
/// TODO: we're kind of mixing C and C++ idioms here
/// TODO: think about all the errors that could happen here
ReadFileResult
read_into_memory(filesystem::path path, void** mem_ptr, u64* max_size_bytes) {
  const auto fsize = std::filesystem::file_size(path);  // throws on err!
  if (fsize > *max_size_bytes) {
    // TODO: can exceptions automatically get logged without repeating messages?
    spdlog::error("Not enough memory to read file");
    throw std::runtime_error("Not enough memory to read file");
  }

  auto file = ifstream(path, ios::binary | ios::in);
  // TODO: is there a stream opener that can throw an exception on error?
  if (!file) {
    spdlog::error("Failed to open file '{}'", static_cast<string>(path));
    throw std::runtime_error("Error opening " + static_cast<string>(path));
  }

  // TODO: could this cast to `char*` cause problems?
  //    Why can't we just read bytes? Does it matter?
  // TODO: error handling here
  file.read(static_cast<char*>(*mem_ptr), fsize);
  const auto result =
      ReadFileResult{.start_ptr = *mem_ptr, .size_bytes = fsize};

  // Clean up and return results
  *mem_ptr = static_cast<uint8*>(*mem_ptr) + fsize;
  *max_size_bytes -= fsize;
  file.close();
  return result;
}