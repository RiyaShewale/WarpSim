
#ifndef PERFORMANCE_ANALYZER_H
#define PERFORMANCE_ANALYZER_H

#include <cstdint>
#include <string>
#include "CacheSimulator.h"

struct PerformanceResult {
    uint64_t threadAccesses = 0;
    uint64_t blockRequests = 0;
    uint64_t memoryBytes = 0;

    CacheStats cacheStats;

    double hitRate = 0.0;
    double blockUtilization = 0.0;
};

PerformanceResult runBenchmark(
    uint64_t stride,
    int repetitions = 2
);

void printBenchmark(
    const std::string& pattern,
    const PerformanceResult& result
);

#endif
