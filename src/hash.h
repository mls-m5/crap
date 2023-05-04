#pragma once

#include <filesystem>
#include <string_view>

namespace crap {

std::string hash(std::string_view str);
std::string hashFile(std::filesystem::path path);

} // namespace crap
