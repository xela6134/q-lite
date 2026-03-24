#include <new>
#include <cstdlib> // For aligned_alloc, free
#include "../memory/pool.hpp"
#include "types.hpp"

// K To Number (length)
// Allocates contiguous memory: [K Struct] -> [Raw Data]
K* ktn(int8_t type, int32_t len) {
    void* memory = pool.alloc(sizeof(K), 8);
    K* z = new(memory) K();
    z->t = type;
    z->n = len;
    z->r = 0;

    size_t element_size = 8;
    size_t bytes = len * element_size;
    size_t aligned_bytes = (bytes + 63) & ~63;

    if (len > 0) {
        void* ptr = pool.alloc(aligned_bytes, 64);

        memset(ptr, 0, aligned_bytes);

        switch (type) {
            case KI: z->I = (int64_t*)ptr; break;
            case KF: z->F = (double*)ptr;  break;
            case KS: z->S = (char**)ptr;   break;
            case KK: z->k = (K**)ptr;      break;
            case XD: z->k = (K**)ptr;      break;
            default: break; 
        }
    }

    return z;
}

// Create Dictionary (xD)
// A Dict is physically a generic list (KK) of 2 items: [Keys, Values]
K* xD(K* keys, K* values) {
    if (keys->n != values->n) return nullptr;

    K* z = ktn(XD, 2); 
    // Note: z->k is already allocated by ktn(XD, 2) above
    
    z->k[0] = keys;    // Index 0: Keys
    z->k[1] = values;  // Index 1: Values
    return z;
}

// Create Table (xT)
// A Table is just a wrapper pointing to a Dictionary
K* xT(K* dict) {
    if (dict->t != XD) return nullptr;
    
    // We manually allocate xT because it doesn't need a buffer, just a pointer
    void* memory = pool.alloc(sizeof(K));
    K* z = new(memory) K();
    z->t = XT;
    z->n = 1;
    z->r = 0;
    z->k0 = dict;   // Point to the dictionary
    return z;
}

// Destructor: Recursively free memory
void r0(K* k) {
    if (!k) return;

    if (k->t == XT) {
        r0(k->k0); 
    } else if (k->t == XD || k->t == KK) {
        for (int i = 0; i < k->n; ++i) {
            r0(k->k[i]);
        }
    }

}

// Utility: Print K object
void show(K* k) {
    if (!k) { std::cout << "(null)\n"; return; }

    if (k->t == KI) {
        std::cout << "[ ";
        for (int i = 0; i < k->n; ++i) std::cout << k->I[i] << " ";
        std::cout << "] (int)" << std::endl;
    } else if (k->t == KF) {
        std::cout << "[ ";
        for (int i = 0; i < k->n; ++i) std::cout << k->F[i] << " ";
        std::cout << "] (float)" << std::endl;
    } else if (k->t == KS) {
        std::cout << "[ ";
        for (int i = 0; i < k->n; ++i) std::cout << "`" << (k->S[i] ? k->S[i] : "null") << " ";
        std::cout << "] (sym)" << std::endl;
    } else if (k->t == KK || k->t == 0) { 
        std::cout << "( " << std::endl;
        for (int i = 0; i < k->n; ++i) {
            std::cout << "  ";
            show(k->k[i]); // Recurse
        }
        std::cout << ")" << std::endl;
    } else if (k->t == XT) {
        std::cout << "+Table" << std::endl;
        show(k->k0); // Recursively show dict
    } else if (k->t == XD) {
        std::cout << "Dictionary:" << std::endl;
        std::cout << "  Keys: "; show(k->k[0]);
        std::cout << "  Vals: "; show(k->k[1]);
    }
}
