#include <gtest/gtest.h>
#include "../../src/core/types.hpp"
#include "../../src/ops/dispatch.hpp"

TEST(OpsTest, VectorizedAdd) {
    int32_t N = 4096; // Large enough to trigger AVX loops
    K* a = ktn(KI, N);
    K* b = ktn(KI, N);

    for (int i = 0; i < N; ++i) {
        a->I[i] = i;
        b->I[i] = 10;
    }

    K* res = add(a, b);

    // Verify first, middle, and last
    EXPECT_EQ(res->I[0], 10);
    EXPECT_EQ(res->I[100], 110);
    EXPECT_EQ(res->I[N-1], N - 1 + 10);

    r0(a); r0(b); r0(res);
}
