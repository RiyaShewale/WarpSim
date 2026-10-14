
#ifndef MEMORY_GENERATOR_H
#define MEMORY_GENERATOR_H

#include <vector>
#include <cstdint>

constexpr int WARP_SIZE = 32;

// Generate contiguous or strided addresses
std::vector<uint64_t> generateAddresses(
    uint64_t warpId,
    uint64_t baseAddress,
    uint64_t stride,
    uint64_t elementSize
);

// Generate random memory addresses
std::vector<uint64_t> generateRandomAddresses(
    uint64_t baseAddress,
    uint64_t numElements,
    uint64_t elementSize,
    uint32_t seed = 42
);

#endif
