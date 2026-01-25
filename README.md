# q-lite

A C++20 vectorised, column-oriented database engine inspired by kdb+. Designed for high-frequency time-series analytics, featuring custom memory management for optimal CPU cache locality and SIMD-accelerated primitives.

## Performance Highlights

| Metric | q-lite Implementation | Standard STL | Improvement |
| --- | --- | --- | --- |
| **Object Allocation** | Bump/Arena Allocator | `new` / `malloc` | **~1200% Faster** |
| **Aggregation** | Custom Hash Map | `std::unordered_map` | **~300% Faster** |
| **Math Ops** | AVX2 Vectorised Kernels | `std::vector` | **~50% Faster** |

## Architecture

### 1. The K-Object Model (`src/core`)

Replaces traditional C++ polymorphism (vtables) with a Tagged Union system. All data types (Atoms, Vectors, Tables) are represented by a unified 32-byte `struct K`.

- **Custom Polymorphism:** Type dispatching is handled via explicit integer tags (`KI`, `KF`, `KB`) rather than virtual function lookups.
- **Cache-Friendly Layout:** `struct K` is aligned to fit exactly two per 64-byte cache line.

### 2. Custom Memory Management (`src/memory`)

Uses a Arena Allocator instead of the general-purpose heap.

- **Spatial Locality:** Objects are allocated sequentially in contiguous RAM blocks, maximizing L1/L2 cache hits during table scans.
- **Zero-Cost Deallocation:** Entire arenas are reset instantly rather than freeing objects individually.

### 3. Vectorised Runtime (`src/ops`)

Query logic is decoupled from storage. The engine uses a dispatcher to route high-level operations (e.g., `add`, `mul`) to type-specific SIMD Kernels.

### 4. Hash-Based Analytics (`src/engine`)

Implements `GROUP BY` and `DISTINCT` using a custom Linear Probing Hash Map.

- **No Pointers:** This map lives in a single flat vector to minimise pointer chasing.
- **Dynamic Resizing:** Uses bitwise masking instead of slow modulo division for hash calculations.

---

## Build & Run

### Prerequisites

- C++20 compliant compiler (GCC 10+, Clang 11+, MSVC 19.28+)
- CMake 3.14+

### Quick Start

```bash
# 1. Clone & Build
mkdir build && cd build
cmake ..
make

# 2. Run Benchmarks
./src/benchmark

# 3. Run Unit Tests (GoogleTest)
ctest

```

## TODOs

**Goals**

- Dictionary
- Table
- Use `mmap`

**Stretch Goals**

- Make it into a REPL (with Q)
- `asof` join (time-series join)
