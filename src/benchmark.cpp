#include <chrono>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "core/types.hpp"
#include "ops/dispatch.hpp"
#include "engine/hashmap.hpp"

using namespace std::chrono;
#define TIME_NOW high_resolution_clock::now()
#define DURATION(start, end) duration_cast<microseconds>(end - start).count()

int main() {
    int64_t N = 10000000; // 10 Million rows (~80MB)
    
    // Benchmark 1: std::vector addition (time: 127607 us)
    {
        auto t1 = TIME_NOW;
        
        std::vector<int64_t> v1(N, 1);
        std::vector<int64_t> v2(N, 2);
        std::vector<int64_t> res(N);
        
        for (size_t i = 0; i < static_cast<size_t>(N); ++i) res[i] = v1[i] + v2[i];
        
        auto t2 = TIME_NOW;
        std::cout << "std::vector time: " << DURATION(t1, t2) << " us" << std::endl;
    }

    // Benchmark 2: K addition (time: 38778 us)
    {
        auto t1 = TIME_NOW;
        
        K* k1 = ktn(KI, N); // No zeroing
        K* k2 = ktn(KI, N);
        K* res = add(k1, k2);

        auto t2 = TIME_NOW;
        std::cout << "q-lite time: " << DURATION(t1, t2) << " us" << std::endl;

        r0(k1); r0(k2); r0(res);
    }

    K* data = ktn(KI, N);
    for (int i = 0; i < N; ++i) data->I[i] = i % 1000;

    // Benchmark 3: std::unordered_set (time: 26388 us)
    {
        auto t1 = TIME_NOW;
        std::unordered_set<int64_t> s;
        for (int i = 0; i < N; ++i) s.insert(data->I[i]);
        auto t2 = TIME_NOW;
        std::cout << "std::unordered_set: " << DURATION(t1, t2) << " us" << std::endl;
    }

    // Benchmark 4: std::unordered_map (time: 24456 us)
    {
        auto t1 = TIME_NOW;
        std::unordered_map<int64_t, int64_t> m;
        for (int i = 0; i < N; ++i) m[data->I[i]] = 1;
        auto t2 = TIME_NOW;
        std::cout << "std::unordered_map: " << DURATION(t1, t2) << " us" << std::endl;
    }

    // Benchmark 5: K distinct (time: 8386 us)
    {
        auto t1 = TIME_NOW;
        K* groups = distinct(data);
        auto t2 = TIME_NOW;
        std::cout << "q-lite time: " << DURATION(t1, t2) << " us" << std::endl;
        r0(groups);
    }

    return 0;
}
