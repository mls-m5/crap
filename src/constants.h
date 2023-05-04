#pragma once

#include <filesystem>

namespace crap {

inline const auto crapRoot = std::filesystem::path{".crap"};
inline const auto potty = crapRoot / "potty";

} // namespace crap
