
#include "CacheSimulator.h"

#include <iostream>
#include <stdexcept>
#include <limits>

using namespace std;


CacheSimulator::CacheSimulator(
    uint64_t cacheSize,
    uint64_t blockSize,
    uint64_t associativity
) : cacheSize(cacheSize),
    blockSize(blockSize),
    associativity(associativity) {

    if (cacheSize == 0 || blockSize == 0 ||
        associativity == 0) {
        throw invalid_argument("Cache parameters must be positive");
    }

    if (cacheSize % blockSize != 0) {
        throw invalid_argument(
            "Cache size must be divisible by block size"
        );
    }

    uint64_t totalLines = cacheSize / blockSize;

    if (totalLines % associativity != 0) {
        throw invalid_argument(
            "Cache lines must be divisible by associativity"
        );
    }

    numSets = totalLines / associativity;

    sets.resize(
        numSets,
        vector<CacheLine>(associativity)
    );
}

bool CacheSimulator::access(uint64_t address) {

    uint64_t blockNumber = address / blockSize;

    uint64_t setIndex = blockNumber % numSets;

    uint64_t tag = blockNumber / numSets;

    auto& currentSet = sets[setIndex];

    
    for (auto& line : currentSet) {

        if (line.valid && line.tag == tag) {

            stats.hits++;
            line.lastUsed = ++timestamp;

            return true;
        }
    }

    
    stats.misses++;

  
    for (auto& line : currentSet) {

        if (!line.valid) {

            line.valid = true;
            line.tag = tag;
            line.lastUsed = ++timestamp;

            return false;
        }
    }

    
    uint64_t lruIndex = 0;

    for (uint64_t i = 1; i < associativity; i++) {

        if (currentSet[i].lastUsed <
            currentSet[lruIndex].lastUsed) {

            lruIndex = i;
        }
    }

    
    stats.evictions++;

    currentSet[lruIndex].valid = true;
    currentSet[lruIndex].tag = tag;
    currentSet[lruIndex].lastUsed = ++timestamp;

    return false;
}

CacheStats CacheSimulator::getStats() const {
    return stats;
}


void CacheSimulator::printStats() const {

    uint64_t total = stats.hits + stats.misses;

    double hitRate = 0.0;

    if (total > 0) {
        hitRate = 100.0 * stats.hits / total;
    }

    cout << "\n===== CACHE STATISTICS =====\n";
    cout << "Cache hits: " << stats.hits << "\n";
    cout << "Cache misses: " << stats.misses << "\n";
    cout << "Evictions: " << stats.evictions << "\n";
    cout << "Hit rate: " << hitRate << "%\n";
}


void CacheSimulator::reset() {

    for (auto& set : sets) {
        for (auto& line : set) {
            line = CacheLine{};
        }
    }

    stats = CacheStats{};
    timestamp = 0;
}
