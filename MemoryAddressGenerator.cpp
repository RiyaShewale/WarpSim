
#include "MemoryAddressGenerator.h"

#include <random>
#include <stdexcept>
using namespace std;


vector<uint64_t> generateAddresses(
    uint64_t warpId,
    uint64_t baseAddress,
    uint64_t stride,
    uint64_t elementSize
) {
    if (elementSize == 0) {
        throw invalid_argument("Element size must be positive");
    }

    vector<uint64_t> addresses;

    for (int lane = 0; lane < WARP_SIZE; lane++) {

        uint64_t globalThreadId =
            warpId * WARP_SIZE + lane;

        uint64_t address =
            baseAddress + globalThreadId * stride * elementSize;

        addresses.push_back(address);
    }

    return addresses;
}



vector<uint64_t> generateRandomAddresses(
    uint64_t baseAddress,
    uint64_t numElements,
    uint64_t elementSize,
    uint32_t seed
) {
    if (numElements == 0 || elementSize == 0) {
        throw invalid_argument("Invalid memory parameters");
    }

    mt19937 rng(seed);

    uniform_int_distribution<uint64_t> dist(
        0, numElements - 1
    );

    vector<uint64_t> addresses;

    for (int lane = 0; lane < WARP_SIZE; lane++) {

        uint64_t randomIndex = dist(rng);

        uint64_t address =
            baseAddress + randomIndex * elementSize;

        addresses.push_back(address);
    }

    return addresses;
}
