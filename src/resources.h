#ifndef RESOURCES_H
#define RESOURCES_H

#include <filesystem>
#include <string>

std::filesystem::path locate_asset(const std::string& subpath);

#endif  // RESOURCES_H