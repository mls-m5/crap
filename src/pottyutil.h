#pragma once

#include "constants.h"
#include <fstream>
#include <string>

namespace crap {

inline std::string butHash() {
    auto file = std::ifstream{butPath};
    std::string line;

    std::getline(file, line);

    return line;
}

} // namespace crap
