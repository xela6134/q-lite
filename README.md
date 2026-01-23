# q-lite

A C++20 vectorized, column-oriented database engine inspired by kdb+. Designed for high-frequency time-series analytics, featuring custom memory management for optimal CPU cache locality and SIMD-accelerated primitives.

## Features

**Vectorized Execution**: Processing logic is decoupled from row-level iteration, applying operations over contiguous memory blocks to maximize SIMD instruction usage AVX2/AVX-512.

**Cache-Aware Architecture**: Engineered with a strict focus on L1/L2 cache optimization. Data is stored in contiguous columnar arrays, minimizing pointer chasing and cache misses during table scans.

**High-Performance Analytics**: Supports fast `GROUP BY` operations and aggregations using hash-based grouping, optimized for large-scale time-series datasets.

## Architecture

The system is built on a custom type system and memory manager to minimize reliance on STL in hot paths.

**K-Object Model**: A lightweight, unified tagged-union structure that represents all data types (atoms, lists, dictionaries), enabling dynamic typing without the overhead of virtual function tables, fits in perfectly in 64-bit (or even 32-bit systems)

**Memory Pool**: A custom slab allocator that manages raw memory arenas, ensuring data locality and reducing OS-level allocation overhead during query execution.

**Query Engine**: A functional interpreter that dispatches operations to optimized C++ kernels based on column types.

## Quick Start

### Prerequisites
- C++ Compiler with C++20 support (GCC/Clang)
- CMake 3.14

### Build
```bash
mkdir -p build && cd build
cmake ..
make
```

### Run Server
```bash
./src/qlite
```

### Run Tests
```bash
./tests/unit_tests
```
