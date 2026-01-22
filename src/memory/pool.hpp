#pragma once
#include <cstddef>
#include <vector>
#include <cstdlib>
#include <iostream>

// Simple Arena Allocator
// Big chunk of memory we can use
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
        // Alignment (8 bytes)
        size_t padding = (8 - (blocks[current_block_idx].used % 8)) % 8;
        size += padding;

        if (blocks[current_block_idx].used + size > BLOCK_SIZE) {
            allocate_new_block();
        }

        char* ptr = blocks[current_block_idx].data + blocks[current_block_idx].used;
        blocks[current_block_idx].used += size;
        
        return (void*)ptr;
    }
};

// Global instance
inline MemoryPool pool;
