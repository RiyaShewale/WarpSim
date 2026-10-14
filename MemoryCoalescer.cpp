
#include "MemoryCoalescer.h"

#include <set>
#include <stdexcept>
using namespace std;

vector<uint64_t> coalesceAddresses(
    const vector<uint64_t>& addresses,
    uint64_t blockSize
) {
    if (blockSize == 0) {
        throw invalid_argument("Block size must be positive");
    }

    set<uint64_t> uniqueBlocks;

    for (uint64_t address : addresses) {

        uint64_t blockIndex = address / blockSize;

        uint64_t blockStart = blockIndex * blockSize;

        uniqueBlocks.insert(blockStart);
    }

    return vector<uint64_t>(
        uniqueBlocks.begin(),
        uniqueBlocks.end()
    );
}
