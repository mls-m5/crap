#pragma once

#include <filesystem>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace crap {

struct Args {
    std::vector<std::string> args;
    std::string method;

    using FT = void (*)(const Args &settings);

    struct CommandInfo {
        std::string name;
        FT f;
        std::string_view description;
    };

    Args(int argc, char *argv[]);

    void locateRoot();

    [[noreturn]] static void printHelp(int code);
};

} // namespace crap
