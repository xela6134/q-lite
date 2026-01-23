#include <chrono>
#include <vector>

#include "core/types.hpp"
#include "ops/dispatch.hpp"

// Simple Timer
using namespace std::chrono;
#define TIME_NOW high_resolution_clock::now()
#define DURATION(start, end) duration_cast<microseconds>(end - start).count()

int main() {
    int64_t N = 10000000; // 10 Million rows (~80MB)
    
    // Benchmark 1: STL Vector (std::vector time: 127607 us)
    {
        auto t1 = TIME_NOW;
        
        std::vector<int64_t> v1(N, 1);
        std::vector<int64_t> v2(N, 2);
        std::vector<int64_t> res(N);
        
        for (size_t i = 0; i < static_cast<size_t>(N); ++i) res[i] = v1[i] + v2[i];
        
        auto t2 = TIME_NOW;
        std::cout << "std::vector time: " << DURATION(t1, t2) << " us" << std::endl;
    }

    // Benchmark 2: q-lite K-Objects (q-lite time: 38778 us)
    {
        auto t1 = TIME_NOW;
        
        K* k1 = ktn(KI, N); // No zeroing
        K* k2 = ktn(KI, N);
        K* res = add(k1, k2);

        auto t2 = TIME_NOW;
        std::cout << "q-lite time: " << DURATION(t1, t2) << " us" << std::endl;
        
        r0(k1); r0(k2); r0(res);
    }

    return 0;
}