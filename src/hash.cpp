#include "hash.h"

#include "hash/sha256.h"
#include <array>
#include <fstream>
#include <iomanip>
#include <ios>
#include <string>

namespace {

std::string toHex(std::string_view binary_data) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');

    for (unsigned char c : binary_data) {
        ss << std::setw(2) << static_cast<int>(c);
    }

    return ss.str();
}

} // namespace

std::string crap::hash(std::string_view data) {
    blk_SHA256_CTX ctx;

    ctx.update(data);
    auto hashStr = ctx.final();

    return toHex(std::string{hashStr.begin(), hashStr.end()});
}

std::string crap::hashFile(std::filesystem::path path) {
    constexpr size_t bufferSize = 4096;
    std::array<char, bufferSize> buffer;

    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + path.string());
    }

    blk_SHA256_CTX ctx;
    while (file.read(buffer.data(), buffer.size())) {
        std::streamsize bytesRead = file.gcount();
        ctx.update(
            std::string_view{buffer.data(), static_cast<size_t>(bytesRead)});
    }

    // Process remaining bytes if any
    std::streamsize bytesRead = file.gcount();
    if (bytesRead > 0) {
        ctx.update(
            std::string_view{buffer.data(), static_cast<size_t>(bytesRead)});
    }

    file.close();

    auto hashStr = ctx.final();
    return toHex(std::string{hashStr.begin(), hashStr.end()});
}
