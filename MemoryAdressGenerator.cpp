#include <iostream>
#include <vector>
#include <cstdint>
#include <random>
#include <stdexcept>
#include <cassert>
using namespace std;

const int WARP_SIZE = 32;

vector<uint64_t> generateAddresses(
    uint64_t warpId,
    uint64_t baseAddress,
    uint64_t stride,
    uint64_t elementSize
) {
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

void printAddresses(const vector<uint64_t>& addresses) {
    for (int i = 0; i < addresses.size(); i++) {
        cout << "Thread " << i
             << " -> Address " << addresses[i] << "\n";
    }
}

vector<uint64_t> generateRandomAddresses(
    uint64_t baseAddress,
    uint64_t numElements,
    uint64_t elementSize,
    uint32_t seed = 42
) {
    if (numElements == 0) {
        throw invalid_argument("numElements must be positive");
    }

    mt19937 rng(seed);

    uniform_int_distribution<uint64_t> dist(0, numElements - 1);

    vector<uint64_t> addresses;

    for (int lane = 0; lane < WARP_SIZE; lane++) {

        uint64_t randomIndex = dist(rng);

        uint64_t address =
            baseAddress + randomIndex * elementSize;

        addresses.push_back(address);
    }

    return addresses;
}


int main() {

    cout << "CONTIGUOUS ACCESS\n";

    auto contiguous = generateAddresses(0, 0, 1, 4);
    printAddresses(contiguous);

    cout << "\nSTRIDED ACCESS\n";

    auto strided = generateAddresses(0, 0, 4, 4);
    printAddresses(strided);

    cout << "\nRANDOM ACCESS\n";

    auto randomAddresses = generateRandomAddresses(0, 1024, 4);
    printAddresses(randomAddresses);


    // Test 1: Contiguous addresses
auto test1 = generateAddresses(0, 0, 1, 4);

assert(test1.size() == 32);
assert(test1[0] == 0);
assert(test1[1] == 4);
assert(test1[31] == 124);

// Test 2: Strided addresses
auto test2 = generateAddresses(0, 0, 4, 4);

assert(test2[0] == 0);
assert(test2[1] == 16);
assert(test2[31] == 496);

// Test 3: Second warp
auto test3 = generateAddresses(1, 0, 1, 4);

assert(test3[0] == 128);
assert(test3[31] == 252);

// Test 4: Your example
auto test4 = generateAddresses(0, 1000, 2, 4);

assert(test4[3] == 1024);

// Test 5: Random addresses stay within bounds
auto test5 = generateRandomAddresses(0, 1024, 4);

assert(test5.size() == 32);

for (auto address : test5) {
    assert(address < 4096);
    assert(address % 4 == 0);
}

cout << "All tests passed!\n";
    return 0;
}