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

TEST(CoreTest, PoolAllocatorHeaderSequentiality) {
    // Allocate objects with zero length
    K* k1 = ktn(KI, 0);
    K* k2 = ktn(KI, 0);
    
    uintptr_t p1 = (uintptr_t)k1;
    uintptr_t p2 = (uintptr_t)k2;
    
    // 16 bytes apart
    long diff = p2 - p1;
    EXPECT_EQ(diff, 16); 

    r0(k1);
    r0(k2);
}

TEST(CoreTest, FlatMemoryArchitecture) {
    K* k = ktn(KI, 1);

    uintptr_t header_addr = (uintptr_t)k;
    uintptr_t payload_addr = (uintptr_t)k->I;
    
    // Verify payload is physically after the header
    EXPECT_GT(payload_addr, header_addr);
    
    // Verify its in the same general memory neighborhood 
    // Header (32) + Padding (32) = 64 bytes
    long diff = payload_addr - header_addr;
    EXPECT_LE(diff, 64);

    // Verify payload is perfectly 64-byte aligned for AVX-512
    EXPECT_EQ(payload_addr % 64, 0);

    r0(k);
}