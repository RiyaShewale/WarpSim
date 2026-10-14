
#ifndef MEMORY_COALESCER_H
#define MEMORY_COALESCER_H

#include <vector>
#include <cstdint>

constexpr uint64_t BLOCK_SIZE = 128;

// Group memory addresses into unique aligned blocks
std::vector<uint64_t> coalesceAddresses(
    const std::vector<uint64_t>& addresses,
    uint64_t blockSize
);

#endif
