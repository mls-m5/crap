#include "droppings.h"
#include "hash.h"
#include "pottyutil.h"

namespace crap {

std::filesystem::path addDropping(std::filesystem::path path) {
    auto hash = hashFile(path);
    auto dpath = droppingsPath(hash);
    if (!std::filesystem::exists(dpath)) {
        std::ofstream{dpath} << std::ifstream{path}.rdbuf();
    }

    return dpath;
}

} // namespace crap
