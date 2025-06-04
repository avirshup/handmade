#include "./resources.h"
#include <filesystem>
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
