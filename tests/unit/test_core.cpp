#include <gtest/gtest.h>
#include "../../src/core/types.hpp"
#include "../../src/memory/pool.hpp"

TEST(CoreTest, KtnAllocatesCorrectType) {
    K* k = ktn(KI, 10);
    ASSERT_EQ(k->t, KI);
    ASSERT_EQ(k->n, 10);
    ASSERT_NE(k->I, nullptr); // Payload exists

    r0(k);
}

TEST(CoreTest, PoolAllocatorSequentiality) {
    K* k1 = ktn(KI, 1);
    K* k2 = ktn(KI, 1);
    
    // Convert pointers to integers to check distance
    uintptr_t p1 = (uintptr_t)k1;
    uintptr_t p2 = (uintptr_t)k2;
    
    // Expect p1 and p2 to be close (size of K is 32 bytes + padding)
    long diff = p2 - p1;
    EXPECT_LT(diff, 64);
    EXPECT_GT(diff, 0);

    r0(k1);
    r0(k2);
}
