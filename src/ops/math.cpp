#include "../core/types.hpp"
#include <stdexcept>
#include <immintrin.h>

const int64_t STREAM_THRESHOLD = 500000;

// Integer Addition Kernel
// The compiler sees this loop and turns it into hyperoptimised instructions with -O3 flag
void add_I(int64_t* __restrict res, const __restrict int64_t* a, const __restrict int64_t* b, int32_t n) {
    int32_t i = 0;
    /**
     * Design Decisions:
     * - With -O3 flag, normal addition & add_I both used hyperoptimised AVX2 instructions anyways, not much difference
     * - Standard Stores)
     *   -> Normal `res[i] = v1[i] + v2[i];` CPU first reads `res` from memory into cache (Read for Ownership), modifies it, then writes back to memory
     *   -> First read is wasted because we're going to modify all bytes anyway
     * - Streaming Stores)
     *   -> `_mm256_stream_si256` bypasses the cache and writes directly to RAM, never reads `res` into cache first
     *
     * This approach is only faster if data is larger than CPU cache, 
     * because if all the data fits in cache, writing to cache is much faster than writing to memory directly.
     */

    if (n > STREAM_THRESHOLD) {
        // Big data (streaming stores)
        for (; i <= n - 4; i += 4) {
            __m256i va = _mm256_load_si256((__m256i const*)&a[i]);
            __m256i vb = _mm256_load_si256((__m256i const*)&b[i]);
            __m256i vres = _mm256_add_epi64(va, vb);

            // Bypass Cache -> RAM 
            // this instruction ASSUMES pointer is 32-byte aligned, so ktn() must guarantee 64 bytes (or 32 bytes) alignment
            _mm256_stream_si256((__m256i*)&res[i], vres);
        }
        _mm_sfence(); // Essential after streaming
    } else {
        // Small data (normal stores)
        for (; i <= n - 4; i += 4) {
            __m256i va = _mm256_load_si256((__m256i const*)&a[i]);
            __m256i vb = _mm256_load_si256((__m256i const*)&b[i]);
            __m256i vres = _mm256_add_epi64(va, vb);

            // Write to cache
            _mm256_store_si256((__m256i*)&res[i], vres); 
        }
    }

    // Cleanup tail (Scalar)
    for (; i < n; ++i) {
        res[i] = a[i] + b[i];
    }
}

// Float Addition Kernel
// Also becomes hyperoptimised with -O3 flag
void add_F(double* res, const double* a, const double* b, int32_t n) {
    for (int32_t i = 0; i < n; ++i) {
        res[i] = a[i] + b[i];
    }
}
