#pragma once

#include <filesystem>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace crap {

struct Args {
    using FT = int (*)(const Args &settings);

    struct CommandInfo {
        std::string_view name;
        Args::FT f;
        std::string_view description;
    };

    using CommandInfos = std::vector<CommandInfo>;

    std::vector<std::string> args;
    std::string method;
    const CommandInfos &commandInfos;

    Args(int argc, char *argv[], const CommandInfos &);

    void locateRoot();

    [[noreturn]] void printHelp(int code);
};

} // namespace crap
