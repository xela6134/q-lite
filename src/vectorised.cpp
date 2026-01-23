#include "core/types.hpp"
#include "ops/dispatch.hpp"

int main() {
    int32_t N = 100;

    K* col_A = ktn(KI, N);
    K* col_B = ktn(KI, N);

    for (int i = 0; i < N; ++i) {
        col_A->I[i] = i;
        col_B->I[i] = 10;
    }

    K* result = add(col_A, col_B);

    std::cout << "A: "; show(col_A);
    std::cout << "B: "; show(col_B);
    std::cout << "Result (A + B): "; show(result);

    r0(col_A);
    r0(col_B);
    r0(result);

    return 0;
}
