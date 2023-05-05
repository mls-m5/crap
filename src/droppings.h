#pragma once

#include <filesystem>

namespace crap {

/// Add a undropped file to droppings
/// @return the path to the dropping
std::filesystem::path addDropping(std::filesystem::path path);

} // namespace crap
