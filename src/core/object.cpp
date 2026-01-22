#include "types.hpp"

// K To Number (length)
// Allocates contiguous memory: [K Struct] -> [Raw Data]
K* ktn(int8_t type, int64_t len) {
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
void r0(K* k) {
    if (!k) return;
    if (k->t == KI) delete[] k->I;
    if (k->t == KF) delete[] k->F;
    delete k;
}

// Utility: Print K object
void show(K* k) {
    if (k->t == KI) {
        std::cout << "type: " << (int)k->t << ", len: " << k->n << " | [ ";
        for(int i=0; i<k->n; ++i) std::cout << k->I[i] << " ";
        std::cout << "]" << std::endl;
    }
}
