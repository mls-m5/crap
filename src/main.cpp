#include "args.h"
#include "init.h"
#include "status.h"
#include <string_view>
#include <unordered_map>

using namespace crap;

namespace {

void commit(const Args &settings) {
    fmt::print("Flushing changes...\n");
}

void flush(const Args &settings) {
    fmt::print("Flushing changes...\n");
}

void shame(const Args &settings) {
    fmt::print("Shame on you\n");
}

} // namespace

int main(int argc, char *argv[]) {
    Args settings(argc, argv);

    auto map = std::unordered_map<std::string, Args::FT>{
        {"commit", commit},
        {"flush", flush},
        {"status", status},
        {"init", init},
    };

    if (auto f = map.find(settings.method); f != map.end()) {
        f->second(settings);
        return 0;
    }

    fmt::print(std::cerr, "invalid command: {}", settings.method);
    settings.printHelp(1);
}
