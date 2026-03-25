#pragma once
#include <cstddef>
#include <vector>
#include <iostream>
#include <sys/mman.h> // Required for mmap, munmap
#include <unistd.h>   // Required for getting OS page size

class MemoryPool {
public:
    MemoryPool() {
        allocate_new_block();
    }

    ~MemoryPool() {
        for (auto& b : blocks) {
            munmap(b.data, b.size);
        }
    }

    void allocate_new_block(size_t required_size = 0) {
        size_t alloc_size = BLOCK_SIZE;
        if (required_size > alloc_size) {
            alloc_size = (required_size + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
        }

        // If this fails
        // Check: cat /proc/meminfo | grep HugePages
        // Run: sudo sysctl -w vm.nr_hugepages=64
        void* ptr = mmap(nullptr, alloc_size, PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);

        // Fall back to regular pages (no big page available)
        if (ptr == MAP_FAILED) {
            ptr = mmap(nullptr, alloc_size, PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            
            if (ptr != MAP_FAILED) {
                // soft request (advising)
                madvise(ptr, alloc_size, MADV_HUGEPAGE);
            }
        }

        if (ptr == MAP_FAILED) {
            std::cerr << "CRITICAL: mmap failed to allocate " << alloc_size << " bytes" << std::endl;
            exit(EXIT_FAILURE);
        }

        blocks.push_back({(char*)ptr, alloc_size, 0});
        ++current_block_idx;
    }

    void* alloc(size_t size, size_t alignment = 8) {
        size_t current_addr = (size_t)(blocks[current_block_idx].data + blocks[current_block_idx].used);
        size_t padding = (0 - current_addr) & (alignment - 1); 

        if (blocks[current_block_idx].used + padding + size > blocks[current_block_idx].size) {
            allocate_new_block(padding + size); 

            current_addr = (size_t)(blocks[current_block_idx].data);
            padding = (0 - current_addr) & (alignment - 1);
        }

        char* ptr = blocks[current_block_idx].data + blocks[current_block_idx].used + padding;
        blocks[current_block_idx].used += (padding + size);

        return (void*)ptr;
    }
private:
    struct Block {
        char* data;
        size_t size;
        size_t used;
    };
    
    std::vector<Block> blocks;
    size_t current_block_idx = -1;
    
    // 2MB pages for mmap (skip 4th level)
    const size_t BLOCK_SIZE = 2 * 1024 * 1024; 
};

inline MemoryPool pool;
