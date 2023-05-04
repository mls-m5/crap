/// Note. This file is more or less copied from the git repository
/// For more information check https://github.com/git/git/
/// It is converted to have some kind of c++ syntax

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>

#define blk_SHA256_BLKSIZE 64

struct blk_SHA256_CTX {
    uint32_t state[8];
    uint64_t size;
    uint32_t offset;
    uint8_t buf[blk_SHA256_BLKSIZE];

    blk_SHA256_CTX();
    void update(std::string_view data);
    //    void update(std::basic_string_view<unsigned char> data);

    std::array<unsigned char, 32> final();
};

typedef struct blk_SHA256_CTX blk_SHA256_CTX;

// void blk_SHA256_Init(blk_SHA256_CTX *ctx);
// void blk_SHA256_Update(blk_SHA256_CTX *ctx, const void *data, size_t len);
// void blk_SHA256_Final(unsigned char *digest, blk_SHA256_CTX *ctx);

// #define platform_SHA256_CTX blk_SHA256_CTX
// #define platform_SHA256_Init blk_SHA256_Init
// #define platform_SHA256_Update blk_SHA256_Update
// #define platform_SHA256_Final blk_SHA256_Final
