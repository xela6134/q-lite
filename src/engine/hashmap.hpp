#pragma once
#include <vector>
#include <cstdint>
#include <cstring>
#include <algorithm>

K* distinct(K* list);

// Optimised Flat Map with Resizing & Bitwise Hashing
class IntHashMap {
    struct Slot {
        int64_t key;
        int64_t val;
        bool used;
    };

    std::vector<Slot> slots;
    size_t capacity;
    size_t size;
    size_t mask;

public:
    IntHashMap(size_t initial_cap = 1024) : size(0) {
        capacity = 1;
        while (capacity < initial_cap) capacity <<= 1; 
        
        slots.resize(capacity, {0, 0, false});
        mask = capacity - 1;
    }

    /** Resize the hash map to a new capacity */
    void resize(size_t new_cap) {
        std::vector<Slot> old_slots = slots;

        // Reset and resize
        slots.clear();
        slots.resize(new_cap, {0, 0, false});
        capacity = new_cap;
        mask = capacity - 1;
        size = 0;

        // Re-insert data
        for (const auto& s : old_slots) {
            if (s.used) {
                put_internal(s.key, s.val);
            }
        }
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

    // Internal helper that doesn't trigger resize (prevents infinite loop)
    int64_t* put_internal(int64_t key, int64_t default_val) {
        // Fast hashing (Knuth multiplicative hash), no modulo
        size_t h = (key * 2654435761);
        size_t idx = h & mask;

        while (true) {
            if (slots[idx].used && slots[idx].key == key) {
                return &slots[idx].val;
            }

            if (!slots[idx].used) {
                slots[idx].key = key;
                slots[idx].used = true;
                slots[idx].val = default_val;
                size++;
                return &slots[idx].val;
            }

            // Linear probing
            idx = (idx + 1) & mask;
        }
    }
    
    std::vector<int64_t> keys() {
        std::vector<int64_t> k;
        k.reserve(size);
        for (const auto& s : slots) {
            if(s.used) k.push_back(s.key);
        }
        return k;
    }
};
