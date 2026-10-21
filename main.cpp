
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <utility>

#include "MemoryAddressGenerator.h"
#include "MemoryCoalescer.h"
#include "CacheSimulator.h"
#include "PerformanceAnalyzer.h"

using namespace std;

// Display generated addresses and coalesced blocks
void runExperiment(
    const string& pattern,
    const vector<uint64_t>& addresses
) {
    cout << "\n========== " << pattern << " ==========\n";

    for (size_t i = 0; i < addresses.size(); i++) {
        cout << "Thread " << i
             << " -> Address " << addresses[i] << "\n";
    }

    auto blocks = coalesceAddresses(addresses, BLOCK_SIZE);

    cout << "\nUnique memory blocks: " << blocks.size() << "\n";
    cout << "Block starting addresses: ";

    for (uint64_t block : blocks) {
        cout << block << " ";
    }

    cout << "\n";
}

// Demonstrate cache hits and misses
void demonstrateCache() {
    cout << "\n========== CACHE SIMULATION ==========\n";

    CacheSimulator cache(1024, BLOCK_SIZE, 2);

    auto addresses = generateAddresses(0, 0, 1, 4);
    auto blocks = coalesceAddresses(addresses, BLOCK_SIZE);

    for (int round = 1; round <= 2; round++) {
        cout << "\nAccess Round " << round << ":\n";

        for (uint64_t block : blocks) {
            bool hit = cache.access(block);

            cout << "Block " << block
                 << (hit ? " -> HIT\n" : " -> MISS\n");
        }
    }

    cache.printStats();
}

// Display performance comparison
void printComparisonTable() {
    vector<pair<string, uint64_t>> patterns = {
        {"Contiguous", 1},
        {"Stride 4", 4},
        {"Stride 32", 32}
    };

    cout << "\n========== PERFORMANCE COMPARISON ==========\n\n";

    cout << fixed << setprecision(2);

    cout << left << setw(16) << "Pattern"
         << right << setw(10) << "Requests"
         << setw(8) << "Hits"
         << setw(9) << "Misses"
         << setw(11) << "Evictions"
         << setw(11) << "Hit %"
         << setw(12) << "Block %"
         << setw(14) << "Est. Bytes"
         << "\n";

    cout << string(91, '-') << "\n";

  for (const auto& p : patterns) {
    string name = p.first;
    uint64_t stride = p.second;

    auto result = runBenchmark(stride, 2);

        cout << left << setw(16) << name
             << right << setw(10) << result.blockRequests
             << setw(8) << result.cacheStats.hits
             << setw(9) << result.cacheStats.misses
             << setw(11) << result.cacheStats.evictions
             << setw(11) << result.hitRate
             << setw(12) << result.blockUtilization
             << setw(14) << result.memoryBytes
             << "\n";
    }

    cout << "\n";
}

int main() {

    
    runExperiment(
        "CONTIGUOUS",
        generateAddresses(0, 0, 1, 4)
    );

    runExperiment(
        "STRIDE 4",
        generateAddresses(0, 0, 4, 4)
    );

    runExperiment(
        "STRIDE 32",
        generateAddresses(0, 0, 32, 4)
    );

    runExperiment(
        "MISALIGNED",
        generateAddresses(0, 4, 1, 4)
    );

    runExperiment(
        "RANDOM",
        generateRandomAddresses(0, 1024, 4)
    );

    
    demonstrateCache();

    printComparisonTable();

    return 0;
}
