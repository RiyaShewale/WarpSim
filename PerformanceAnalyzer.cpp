
#include "PerformanceAnalyzer.h"
#include "MemoryAddressGenerator.h"
#include "MemoryCoalescer.h"

#include <iostream>
#include <iomanip>
#include <stdexcept>

using namespace std;

PerformanceResult runBenchmark(
    uint64_t stride,
    int repetitions
) {
    if (stride == 0 || repetitions <= 0) {
        throw invalid_argument("Invalid benchmark parameters");
    }


    CacheSimulator cache(1024, BLOCK_SIZE, 2);

    PerformanceResult result;

    auto addresses = generateAddresses(0, 0, stride, 4);

   
    auto blocks = coalesceAddresses(addresses, BLOCK_SIZE);

  
    for (int i = 0; i < repetitions; i++) {

        result.threadAccesses += addresses.size();
        result.blockRequests += blocks.size();

        for (uint64_t block : blocks) {
            cache.access(block);
        }
    }

    result.cacheStats = cache.getStats();

    uint64_t total =
        result.cacheStats.hits + result.cacheStats.misses;

    if (total > 0) {
        result.hitRate =
            100.0 * result.cacheStats.hits / total;
    }


    double usefulBytes = result.threadAccesses * 4.0;

    double requestedBlockBytes =
        result.blockRequests * static_cast<double>(BLOCK_SIZE);

    if (requestedBlockBytes > 0) {
        result.blockUtilization =
            100.0 * usefulBytes / requestedBlockBytes;
    }

    result.memoryBytes =
        result.cacheStats.misses * BLOCK_SIZE;

    return result;
}

void printBenchmark(
    const string& pattern,
    const PerformanceResult& result
) {
    cout << "\n========== " << pattern << " ==========\n";

    cout << "Thread accesses: "
         << result.threadAccesses << "\n";

    cout << "Coalesced block requests: "
         << result.blockRequests << "\n";

    cout << "Cache hits: "
         << result.cacheStats.hits << "\n";

    cout << "Cache misses: "
         << result.cacheStats.misses << "\n";

    cout << "Evictions: "
         << result.cacheStats.evictions << "\n";

    cout << fixed << setprecision(2);

    cout << "Cache hit rate: "
         << result.hitRate << "%\n";

    cout << "Block utilization: "
         << result.blockUtilization << "%\n";

    cout << "Modelled memory traffic: "
         << result.memoryBytes << " bytes\n";
}
