
#include "commit.h"
#include "constants.h"
#include "pottyutil.h"
#include "status.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <ranges>
#include <string>
#include <string_view>

namespace crap {

Commit::Commit(std::string hash) {
    auto file = std::ifstream{commitPath(hash)};

    std::getline(file, parent);

    std::string numFiles;
    std::getline(file, numFiles);

    if (numFiles.empty()) {
        return;
    }

    auto num = std::stol(numFiles);

    auto split = [](std::string_view str) {
        auto s = str.find(" ");

        return std::pair{str.substr(0, s), str.substr(s + 1)};
    };

    for (size_t i = 0; i < num; ++i) {
        std::string line;
        std::getline(file, line);

        auto [hash, name] = split(line);

        files.push_back({std::string{hash}, name, droppingsPath(name)});
    }

    for (std::string line; std::getline(file, line);) {
        message += line + "\n";
    }

    while (!message.empty() && std::isspace(message.back())) {
        message.pop_back();
    }

    while (!message.empty() && std::isspace(message.front())) {
        message.erase(0, 1);
    }

    //    std::ranges::sort(files, [](auto &a, auto &b) { return a.path <
    //    b.path; });
}

Commit::Commit(std::string parent, std::vector<File> files, std::string message)
    : parent{std::move(parent)}
    , files{std::move(files)}
    , message{std::move(message)} {}

Commit stagedFiles(const Status &status) {
    auto filesView =
        status.staged | std::views::transform([](std::filesystem::path path) {
            return Commit::File{};
        });

    return Commit{butHash(),
                  std::vector<Commit::File>{filesView.begin(), filesView.end()},
                  ""};
}

} // namespace crap
