#pragma once
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdint>

// Type Tags
// < 0 : atom (scalar)
// = 0 : mixed list
// > 0 : typed list (vector)
constexpr int8_t KB = 1;  // boolean
constexpr int8_t KI = 4;  // integer (64-bit)
constexpr int8_t KF = 9;  // float (double)

struct K {
    // Headers
    int8_t t;   // type
    int8_t a;   // attributes (sorted, etc.)
    int16_t r;  // ref count
    int64_t n;  // number of elements (for vectors)

    /**
     * Payload
     * 
     * Uses an anonymous union
     * Normally, to handle int, float, etc in one object, we need to use inheritance or std::variant.
     * This creates vtable overhead -> higher latency & memory usage.
     */
    union {
        int64_t i;      // Scalar integer
        double f;       // Scalar float
        void* v;        // Generic pointer to data
        struct K** k;   // Pointer to mixed list
        int64_t* I;     // Pointer to integer array
        double* F;      // Pointer to float array
    };
};

// K To Number (length)
// Allocates contiguous memory: [K Struct] -> [Raw Data]
inline K* ktn(int8_t type, int64_t len) {
    // Allocate K struct
    K* z = new K();
    z->t = type;
    z->n = len;
    z->r = 0;

    // Allocate Data Buffer
    // TODO: use mmap/malloc aligned to 64 bytes for AVX
    if (type == KI) z->I = new int64_t[len];
    else if (type == KF) z->F = new double[len];

    return z;
}

// Destructor: Recursively free memory
inline void r0(K* k) {
    if (!k) return;
    if (k->t == KI) delete[] k->I;
    if (k->t == KF) delete[] k->F;
    delete k;
}

// Utility: Print K object
inline void show(K* k) {
    if (k->t == KI) {
        std::cout << "type: " << (int)k->t << ", len: " << k->n << " | [ ";
        for(int i=0; i<k->n; ++i) std::cout << k->I[i] << " ";
        std::cout << "]" << std::endl;
    }
}
