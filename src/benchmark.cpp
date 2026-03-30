#include <chrono>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "core/types.hpp"
#include "ops/dispatch.hpp"
#include "engine/hashmap.hpp"
#include "memory/pool.hpp"

using namespace std::chrono;
#define TIME_NOW high_resolution_clock::now()
#define DURATION(start, end) duration_cast<microseconds>(end - start).count()

int main() {
    int32_t N = 50000000;   // 50 Million rows (~400 MB)
                            // Also performs well with smaller sizes under STREAM_THRESHOLD defined in ops/math.cpp

    // Benchmark 1: std::vector addition (time: 50939 us)
    {
        std::vector<int64_t> v1(N, 1);
        std::vector<int64_t> v2(N, 2);
        std::vector<int64_t> res(N, 0);

        size_t sizeN = static_cast<size_t>(N);

        auto t1 = TIME_NOW;
        for (size_t i = 0; i < sizeN; ++i) res[i] = v1[i] + v2[i];
        auto t2 = TIME_NOW;

        std::cout << "std::vector time: " << DURATION(t1, t2) << " us" << std::endl;
    }

    // Benchmark 2: K addition (time: 34240 us)
    {
        K* k1 = ktn(KI, N);
        K* k2 = ktn(KI, N);
        K* res = ktn(KI, N); // Just gives the memory to use and doesn't actually load it on RAM

        for (int i = 0; i < N; ++i) {
            k1->I[i] = 1;
            k2->I[i] = 2;
            res->I[i] = 0; // Fun fact: Not having this line causes extreme system overhead (lazy allocation -> lots of page faults)
        }

        auto t1 = TIME_NOW;
        add_I(res->I, k1->I, k2->I, N);
        auto t2 = TIME_NOW;

        std::cout << "K addition time: " << DURATION(t1, t2) << " us" << std::endl;

        r0(k1); r0(k2); r0(res);
    }

    std::cout << std::endl;

    N = 1000000; // 10 Million rows (~80 MB)

    K* data = ktn(KI, N);
    for (int i = 0; i < N; ++i) data->I[i] = i % 1000;

    // Benchmark 3: std::unordered_set (time: 2868 us)
    {
        auto t1 = TIME_NOW;
        std::unordered_set<int64_t> s;
        for (int i = 0; i < N; ++i) s.insert(data->I[i]);
        auto t2 = TIME_NOW;
        std::cout << "std::unordered_set time: " << DURATION(t1, t2) << " us" << std::endl;
    }

    // Benchmark 4: std::unordered_map (time: 2549 us)
    {
        auto t1 = TIME_NOW;
        std::unordered_map<int64_t, int64_t> m;
        for (int i = 0; i < N; ++i) m[data->I[i]] = 1;
        auto t2 = TIME_NOW;
        std::cout << "std::unordered_map time: " << DURATION(t1, t2) << " us" << std::endl;
    }

    // Benchmark 5: K distinct (time: 905 us)
    {
        auto t1 = TIME_NOW;
        K* groups = distinct(data);
        auto t2 = TIME_NOW;
        std::cout << "K distinct time: " << DURATION(t1, t2) << " us" << std::endl;
        r0(groups);
    }

    std::cout << std::endl;

    // Benchmark 6: Standard C++ heap (time: 22271 us)
    {
        std::vector<K*> v; 
        v.reserve(N);

        auto t1 = TIME_NOW;

        for (int i = 0; i < N; ++i) {
            K* k = new K(); // Standard malloc
            k->t = KI;
            k->i = i;
            v.push_back(k);
        }

        auto t2 = TIME_NOW;
        std::cout << "new K() time: " << DURATION(t1, t2) << " us" << std::endl;

        for (K* k : v) delete k; 
    }

    // Benchmark 7: Custom memory pool (time: 2971 us)
    {
        std::vector<K*> v;
        v.reserve(N);

        auto t1 = TIME_NOW;

        for (int i = 0; i < N; ++i) {
            // Direct Pool Access (Simulating an 'Atom' factory)
            // This is just a pointer increment. Extremely fast.
            K* k = (K*) pool.alloc(sizeof(K)); 
            k->t = -KI;
            k->i = i;
            v.push_back(k);
        }

        auto t2 = TIME_NOW;
        std::cout << "K arena allocator time: " << DURATION(t1, t2) << " us" << std::endl;
    }

    r0(data);

    return 0;
}
