
#ifndef CACHE_SIMULATOR_H
#define CACHE_SIMULATOR_H

#include <cstdint>
#include <vector>

struct CacheLine {
    bool valid = false;
    uint64_t tag = 0;
    uint64_t lastUsed = 0;
};

struct CacheStats {
    uint64_t hits = 0;
    uint64_t misses = 0;
    uint64_t evictions = 0;
};

class CacheSimulator {
private:
    uint64_t cacheSize;
    uint64_t blockSize;
    uint64_t associativity;
    uint64_t numSets;

    // Each set contains multiple cache lines (ways)
    std::vector<std::vector<CacheLine>> sets;

    CacheStats stats;
    uint64_t timestamp = 0;

public:
    CacheSimulator(
        uint64_t cacheSize,
        uint64_t blockSize,
        uint64_t associativity
    );

    // Returns true on hit, false on miss
    bool access(uint64_t address);

    void printStats() const;

    CacheStats getStats() const;

    void reset();
};

#endif
