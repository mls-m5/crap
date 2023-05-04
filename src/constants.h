#pragma once

#include <array>
#include <filesystem>

namespace crap {

inline const auto crapRoot = std::filesystem::path{".crap"};
inline const auto pottyPath = crapRoot / "potty";
inline const auto commitPath = crapRoot / "commits";
inline const auto droppingsPath = crapRoot / "droppings";

inline const auto requiredDirectories = std::array{
    pottyPath,
    commitPath,
    droppingsPath,
};

} // namespace crap
