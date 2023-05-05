#pragma once

#include "constants.h"
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>

namespace crap {

inline std::filesystem::path commitPath(std::string_view str) {
    return constants::commitPath / str;
}

inline std::string readFirstLine(const std::filesystem::path &path) {
    auto file = std::ifstream{path};
    std::string line;

    std::getline(file, line);

    return line;
}

inline std::string butHash() {
    return readFirstLine(constants::butPath);
}

inline std::string butParentPath(std::string_view hash) {
    return readFirstLine(commitPath(hash));
}

inline std::string butParentPath() {
    return butParentPath(butHash());
}

// TODO: Remove this. It should not be used any longer
// inline std::filesystem::path pottyPath(std::filesystem::path path) {
//    return constants::pottyPath / path;
//}

inline std::filesystem::path pottyFilePath() {
    return constants::pottyFilePath;
}

inline std::filesystem::path droppingsPath(std::filesystem::path path) {
    return constants::droppingsPath / path;
}

inline std::filesystem::path commitMsgPath() {
    return constants::commitMsgPath;
}

inline bool areFilesDifferent(const std::filesystem::path &file_path1,
                              const std::filesystem::path &file_path2) {
    // Compare file sizes
    auto file_size1 = std::filesystem::file_size(file_path1);
    auto file_size2 = std::filesystem::file_size(file_path2);

    if (file_size1 != file_size2) {
        return true;
    }

    // Compare file contents
    std::ifstream file1(file_path1, std::ios::binary);
    std::ifstream file2(file_path2, std::ios::binary);

    if (!file1.is_open() || !file2.is_open()) {
        throw std::runtime_error(
            "Unable to open one or both files for comparison.");
    }

    constexpr size_t buffer_size = 4096;
    std::array<char, buffer_size> buffer1 = {};
    std::array<char, buffer_size> buffer2 = {};

    while (file1.read(buffer1.data(), buffer_size) &&
           file2.read(buffer2.data(), buffer_size)) {
        if (buffer1 != buffer2) {
            return true;
        }

        file1 = {};
        file2 = {};
    }

    return false;
}

inline std::string cleanMessage(std::string message) {
    auto ss = std::istringstream{std::move(message)};

    message.clear();

    for (std::string line; std::getline(ss, line);) {
        if (line.empty()) {
        }
        else if (line.front() == '#') {
            continue;
        }

        message += line + "\n";
    }

    while (!message.empty() && std::isspace(message.back())) {
        message.pop_back();
    }

    while (!message.empty() && std::isspace(message.front())) {
        message.erase(0, 1);
    }

    return message;
}

} // namespace crap
