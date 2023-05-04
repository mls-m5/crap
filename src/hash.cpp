#include "hash.h"

#include "hash/sha256.h"
#include <array>
#include <iomanip>
#include <ios>
#include <string>

namespace {

std::string to_hex(std::string_view binary_data) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');

    for (unsigned char c : binary_data) {
        ss << std::setw(2) << static_cast<int>(c);
    }

    return ss.str();
}

} // namespace

std::string crap::hash(std::string_view data) {
    //    std::array<unsigned char, 32> hashStr; // 32 bytes for the SHA-256
    //    hash
    blk_SHA256_CTX ctx;

    //    blk_SHA256_Init(&ctx);
    //    blk_SHA256_Update(
    //        &ctx, data.data(), data.size()); // -1 to exclude the null
    //        terminator
    ctx.update(data); // -1 to exclude the null terminator
    auto hashStr = ctx.final();
    //    blk_SHA256_Final(static_cast<unsigned char *>(hashStr.data()), &ctx);

    return to_hex(std::string{hashStr.begin(), hashStr.end()});
}

std::string crap::hashFile(std::filesystem::path path) {}
