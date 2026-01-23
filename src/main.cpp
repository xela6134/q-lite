#include "core/types.hpp"
#include "ops/dispatch.hpp"

int main() {
    std::cout << "Initializing q-lite..." << std::endl;
    // PART 1: Basic functionality
    // Create a vector of 10 integers (Type KI)
    int32_t N = 10;
    K* vec = ktn(KI, N);

    for (int i = 0; i < N; ++i) {
        vec->I[i] = i * 10;
    }

    // Inspect & show the object
    show(vec);
    r0(vec);

    // PART 2: Memory alignment

    // Working memory alignment
    std::cout << "\nWorking Memory Alignment:" << std::endl;
    for (int i = 0; i < 5; ++i) {
        K* z = ktn(KI, 0);

        std::cout << "Object " << i << " Address: 0x"
            << std::hex << (uintptr_t)z << std::dec
            << " (Offset: " << ((uintptr_t)z % 1000) << ")"
            << std::endl;
        
        r0(z);
    }

    // Bad memory alignment
    std::cout << "\nBad Memory Alignment:" << std::endl;
    std::vector<K*> heap_ptrs;

    for (int i = 0; i < 5; ++i) {
        heap_ptrs.push_back(new K());
    }

    for (int i = 0; i < 5; ++i) {
        std::cout << "Object " << i << " Address: 0x" 
                << std::hex << (uintptr_t)heap_ptrs[i] << std::dec 
                << " (Offset: " << ((uintptr_t)heap_ptrs[i] % 1000) << ")"
                << std::endl;
    }

    for (K* k : heap_ptrs) delete k;

    // PART 3 in separate file (vectorised.cpp)

    return 0;
}
