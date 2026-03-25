#pragma once
#include <vector>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <xmmintrin.h>

K* distinct(K* list);

// Optimised Flat Map with Resizing & Bitwise Hashing
class IntHashMap {
public:
    IntHashMap(size_t initial_cap = 1024) : size(0) {
        capacity = 1;
        while (capacity < initial_cap) capacity <<= 1; 
        
        slots.resize(capacity, {EMPTY, 0});
        mask = capacity - 1;
    }

    /**
     * Returns pointer to value
     * WARNING: Do not use returned pointer, has dangling pointer issue if map resizes
     */
    int64_t* get(int64_t key) {
        // Resize if 50% full
        if (size * 2 > capacity) {
            resize(capacity * 2);
        }
        
        return put_internal(key, 0); // 0 is default value
    }

    std::vector<int64_t> keys() {
        std::vector<int64_t> k;
        k.reserve(size);
        for (const auto& s : slots) {
            if (s.key != EMPTY) k.push_back(s.key);
        }
        return k;
    }
private:
    static constexpr int64_t EMPTY = INT64_MIN;

    // Mark key = EMPTY for empty slots
    struct Slot {
        int64_t key;
        int64_t val;
    };

    std::vector<Slot> slots;
    size_t capacity;
    size_t size;
    size_t mask;

    // Internal helper that doesn't trigger resize (prevents infinite loop)
    int64_t* put_internal(int64_t key, int64_t default_val) {
        // Fast hashing (Knuth multiplicative hash), no modulo
        size_t h = (key * 2654435761);
        size_t idx = h & mask;

        while (true) {
            // prefetch
            // _mm_prefetch(&slots[(idx + 1) & mask], _MM_HINT_T0);

            if (slots[idx].key == key) {
                return &slots[idx].val;
            }

            if (slots[idx].key == EMPTY) {
                slots[idx].key = key;
                slots[idx].val = default_val;
                size++;
                return &slots[idx].val;
            }

            // Linear probing
            idx = (idx + 1) & mask;
        }
    }

    /** Resize the hash map to a new capacity */
    void resize(size_t new_cap) {
        std::vector<Slot> old_slots = slots;

        // Reset and resize
        slots.clear();
        slots.resize(new_cap, {EMPTY, 0});
        capacity = new_cap;
        mask = capacity - 1;
        size = 0;

        // Re-insert data
        for (const auto& s : old_slots) {
            if (s.key != EMPTY) {
                put_internal(s.key, s.val);
            }
        }
    }
};
