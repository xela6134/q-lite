#pragma once    // header file included only once during compilation

#include <cstdint>
#include <iostream>
#include <vector>
#include <cstring>

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
    int32_t n;  // number of elements (for vectors)

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

K* ktn(int8_t type, int64_t len);
void r0(K* k);
void show(K* k);
