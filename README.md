# WarpSim — GPU Memory Coalescing & Cache Simulator

WarpSim is a C++17 simulator that models GPU memory access patterns, warp-level memory coalescing, and set-associative cache behavior.

The project explores how memory access patterns affect the number of memory blocks requested, cache hit rates, and estimated memory traffic.

## Features

- **Memory Address Generation:** Generates contiguous, strided, random, and misaligned memory-access patterns for a 32-thread warp.
- **Memory Coalescing:** Groups thread addresses into aligned 128-byte memory blocks.
- **Cache Simulation:** Models a configurable set-associative cache with LRU replacement.
- **Performance Analysis:** Compares block requests, cache hits, misses, evictions, block utilization, and estimated memory traffic.

## Architecture

The simulator consists of four modules:

1. `MemoryAddressGenerator` — Generates memory addresses for simulated GPU threads.
2. `MemoryCoalescer` — Groups the addresses into unique aligned memory blocks.
3. `CacheSimulator` — Simulates cache mapping, hits, misses, and LRU eviction.
4. `PerformanceAnalyzer` — Collects and compares memory-access statistics.

**Execution flow:**

Memory Address Generator → Memory Coalescer → Cache Simulator → Performance Analyzer

## Build and Run

Requires a C++17 compiler such as G++.

Compile:

`g++ -std=c++17 -Wall -Wextra main.cpp MemoryAddressGenerator.cpp MemoryCoalescer.cpp CacheSimulator.cpp PerformanceAnalyzer.cpp -o WarpSim.exe`

Run on Windows:

`.\WarpSim.exe`

The program displays individual thread addresses, coalesced memory blocks, a cache hit/miss demonstration, and a performance comparison table.

## Benchmark Results

Default configuration:

- Warp size: 32 threads
- Element size: 4 bytes
- Cache capacity: 1 KB
- Cache line size: 128 bytes
- Associativity: 2-way
- Replacement policy: LRU
- Workload: Two repetitions of the same warp's memory accesses

| Access Pattern | Block Requests | Hits | Misses | Hit Rate | Block Utilization |
|---|---:|---:|---:|---:|---:|
| Contiguous | 2 | 1 | 1 | 50% | 100% |
| Stride 4 | 8 | 4 | 4 | 50% | 25% |
| Stride 32 | 64 | 0 | 64 | 0% | 3.125% |

Contiguous accesses require fewer memory block requests and achieve higher block utilization. Larger strides spread requests across more blocks, increasing the amount of data fetched per useful byte in this model.

## Limitations

WarpSim is a simplified, block-level software model rather than a cycle-accurate GPU simulator. It assumes 128-byte memory blocks and models a single cache level. It does not simulate actual GPU memory-sector behavior, execution scheduling, or hardware latency.

Estimated memory traffic is calculated as cache misses multiplied by the block size, rather than measured physical GPU memory traffic.
