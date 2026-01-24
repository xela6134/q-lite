#pragma once
#include <cstddef>
#include <vector>
#include <cstdlib>
#include <iostream>

/**
 * Simple area allocator, big chunk of memory we can use
 * Advantage: 64-bit CPU alignment guaranteed
 * Tradeoff: More internal fragmentation for memory
 * NOT a static heap-free memory allocator
 */
class MemoryPool {
    struct Block {
        char* data;
        size_t size;
        size_t used;
    };
    
    std::vector<Block> blocks;
    size_t current_block_idx = -1;
    const size_t BLOCK_SIZE = 1024 * 1024; // 1MB chunk

public:
    MemoryPool() {
        allocate_new_block();
    }

    ~MemoryPool() {
        for (auto& b : blocks) free(b.data);
    }

    void allocate_new_block() {
        char* ptr = (char*) malloc(BLOCK_SIZE);
        blocks.push_back({ptr, BLOCK_SIZE, 0});
        ++current_block_idx;
    }

    void* alloc(size_t size) {
        // Add padding every single time to ensure 8-byte alignment (64-bit systems)
        // e.g. [char] [ ] [ ] [ ] [ ] [ ] [ ] [ ] -> 1 byte char, 7 bytes padding
        size_t current_addr = (size_t)(blocks[current_block_idx].data + blocks[current_block_idx].used);
        size_t padding = (0 - current_addr) & 7; // modulo 8 operation

        // Check capacity including the padding we need to skip
        if (blocks[current_block_idx].used + padding + size > BLOCK_SIZE) {
            allocate_new_block();
            padding = 0; 
        }

        // Move the pointer past padding
        char* ptr = blocks[current_block_idx].data + blocks[current_block_idx].used + padding;
        
        // Update usage
        blocks[current_block_idx].used += (padding + size);
        
        return (void*)ptr;
    }
};

// Global instance
inline MemoryPool pool;
