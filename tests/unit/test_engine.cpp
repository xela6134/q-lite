#include <gtest/gtest.h>
#include "../../src/core/types.hpp"
#include "../../src/engine/hashmap.hpp"

TEST(EngineTest, GroupByDistinct) {
    K* data = ktn(KI, 10);
    // [ 5, 5, 5, 1, 1, 2, 2, 2, 3, 5 ]
    int64_t inputs[] = {5, 5, 5, 1, 1, 2, 2, 2, 3, 5};
    for (int i = 0; i < 10; ++i) data->I[i] = inputs[i];

    K* groups = distinct(data);

    // We expect 4 unique values: 1, 2, 3, 5
    ASSERT_EQ(groups->n, 4);

    // Check content (Order isn't guaranteed in hash map, so use a set to verify)
    std::set<int64_t> result_set;
    for (int i = 0; i < groups->n; ++i) result_set.insert(groups->I[i]);

    EXPECT_TRUE(result_set.contains(1));
    EXPECT_TRUE(result_set.contains(5));
    
    r0(data);
    r0(groups);
}
