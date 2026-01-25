#include <new>

#include "../memory/pool.hpp"
#include "types.hpp"

// K To Number (length)
// Allocates contiguous memory: [K Struct] -> [Raw Data]
K* ktn(int8_t type, int32_t len) {
    // Memory pool allocation
    void* memory = pool.alloc(sizeof(K));

    // Placement New: Skips allocation (finding free memory), just constructs in given memory
    // Normal malloc is allocate + construct. Different from `new K()`
    K* z = new(memory) K();
    z->t = type;
    z->n = len;
    z->r = 0;

    // Memory alignment for payload as 64 bytes
    // Modern CPU caches (L1 ~ L3) are all aligned in 64 bytes

    size_t bytes = len * (type == KI ? 8 : 8);
    size_t aligned_bytes = (bytes + 63) & ~63;

    if (type == KI) {
        z->I = (int64_t*) aligned_alloc(64, aligned_bytes);
    } else if (type == KF) {
        z->F = (double*) aligned_alloc(64, aligned_bytes);
    }

    // if (type == KI) z->I = new int64_t[len];
    // else if (type == KF) z->F = new double[len];

    return z;
}

// Destructor: Recursively free memory
void r0(K* k) {
    if (!k) return;
    if (k->t == KI) delete[] k->I;
    if (k->t == KF) delete[] k->F;
    
    // Do not call k itself since we use arena allocator
    // Supposed to be leaked memory
}

// Utility: Print K object
void show(K* k) {
    if (k->t == KI) {
        std::cout << "type: " << (int)k->t << ", len: " << k->n << " | [ ";
        for (int i = 0; i < k->n; ++i) {
            std::cout << k->I[i] << " ";
        } 
        std::cout << "]" << std::endl;
    }
}
