#include <new>

#include "../memory/pool.hpp"
#include "types.hpp"

// K To Number (length)
// Allocates contiguous memory: [K Struct] -> [Raw Data]
K* ktn(int8_t type, int64_t len) {
    void* memory = pool.alloc(sizeof(K));

    K* z = new(memory) K();
    z->t = type;
    z->n = len;
    z->r = 0;

    if (type == KI) z->I = new int64_t[len]; 
    else if (type == KF) z->F = new double[len];

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
