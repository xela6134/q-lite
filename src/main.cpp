#include "k.hpp"

int main() {
    std::cout << "Initializing q-lite..." << std::endl;

    // Create a vector of 10 integers (Type KI)
    int64_t N = 10;
    K* vec = ktn(KI, N);

    for (int i = 0; i < N; ++i) {
        vec->I[i] = i * 10;
    }

    // Inspect the object
    show(vec);
    
    // Clean up
    r0(vec);

    K* val1 = new K();
    val1->t = KI;
    val1->i = 42;

    std::cout << "Value 1 (Integer): " << val1->i << std::endl;

    K* val2 = new K();
    val2->t = KF;
    val2->f = 3.14159; 
    
    std::cout << "Value 2 (Float): " << val2->f << std::endl;

    // Demonstrate type confusion (unsafe)
    std::cout << "Reading Integer as Float (Garbage): " << val1->f << std::endl;

    delete val1;
    delete val2;

    return 0;
}