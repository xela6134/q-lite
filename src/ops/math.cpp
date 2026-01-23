#include "../core/types.hpp"
#include <stdexcept>

// Integer Addition Kernel
// The compiler sees this loop and turns it into hyperoptimised instructions with -O3 flag
void add_I(int64_t* res, const int64_t* a, const int64_t* b, int32_t n) {
    for (int64_t i = 0; i < n; ++i) {
        res[i] = a[i] + b[i];
    }
}

// Float Addition Kernel
// Also becomes hyperoptimised with -O3 flag
void add_F(double* res, const double* a, const double* b, int32_t n) {
    for (int64_t i = 0; i < n; ++i) {
        res[i] = a[i] + b[i];
    }
}
