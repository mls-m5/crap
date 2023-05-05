#pragma once

#include <array>
#include <filesystem>

namespace crap {

namespace constants {

inline const auto crapRoot = std::filesystem::path{".crap"};
inline const auto pottyPath = crapRoot / "potty";
inline const auto commitPath = crapRoot / "commits";
inline const auto droppingsPath = crapRoot / "droppings";

inline const auto butPath = crapRoot / "BUT";
inline const auto commitMsgPath = crapRoot / "FLUSH_MSG";

inline const auto requiredDirectories = std::array{
    pottyPath,
    commitPath,
    droppingsPath,
};

inline const auto requiredFiles = std::array{
    butPath,
};

} // namespace constants

} // namespace crap
