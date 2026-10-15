
#include "CacheSimulator.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

using namespace std;

int main() {

    // 256 bytes total, 128 bytes per line:
    // one set, two ways
    CacheSimulator cache(256, 128, 2);

    assert(cache.access(0) == false);     // A: miss
    assert(cache.access(128) == false);   // B: miss
    assert(cache.access(0) == true);      // A: hit

    // Cache full. B is now least recently used.
    assert(cache.access(256) == false);   // C: miss, evict B

    // A should remain in the cache
    assert(cache.access(0) == true);

    CacheStats stats = cache.getStats();

    assert(stats.hits == 2);
    assert(stats.misses == 3);
    assert(stats.evictions == 1);

    cout << "LRU replacement test passed!\n";

    // Test reset
    cache.reset();

    stats = cache.getStats();

    assert(stats.hits == 0);
    assert(stats.misses == 0);
    assert(stats.evictions == 0);

    assert(cache.access(0) == false);

    cout << "Cache reset test passed!\n";

    // Invalid configuration
    bool caught = false;

    try {
        CacheSimulator invalid(128, 128, 2);
    } catch (const invalid_argument&) {
        caught = true;
    }

    assert(caught);

    cout << "Cache validation test passed!\n";
    cout << "\nAll cache tests passed!\n";

    return 0;
}
