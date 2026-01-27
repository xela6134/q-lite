#pragma once    // header file included only once during compilation

#include <cstdint>
#include <iostream>
#include <vector>
#include <cstring>

// Type Tags
constexpr int8_t KK = 0;    // Mixed List (List of K objects)
constexpr int8_t KB = 1;    // Boolean
constexpr int8_t KI = 4;    // Integer (64-bit)
constexpr int8_t KF = 9;    // Float (double)
constexpr int8_t KS = 11;   // Symbol (string)
constexpr int8_t XT = 98;   // Table
constexpr int8_t XD = 99;   // Dictionary

// 16 byte struct
struct K {
    // Headers: 1 + 1 + 2 + 4 = 8 bytes
    int8_t t;   // type
    int8_t a;   // attributes (sorted, etc.)
    int16_t r;  // ref count
    int32_t n;  // number of elements (for vectors)

    /**
     * Payload: uses an anonymous union (8 bytes max)
     * - 
     * 
     * Design Decisions:
     * - Normally, to handle int, float, etc in one object, we need to use inheritance or std::variant.
     * - This creates vtable overhead -> higher latency & memory usage.
     * - So we store type, then 
     */
    union {
        int64_t i;      // Scalar integer
        double f;       // Scalar float

        int64_t* I;     // Int Array
        double* F;      // Float Array
        char** S;       // Symbol Array (Array of char*)

        struct K* k0;   // Single K pointer (For Table -> Dict)
        struct K** k;   // Generic list of K objects (for Dict Values)
    };
};

K* ktn(int8_t type, int32_t len);   // Create typed K object with given length
K* xD(K* keys, K* values);          // Create dictionary
K* xT(K* dict);                     // Create table (Flip)

void r0(K* k);
void show(K* k);
